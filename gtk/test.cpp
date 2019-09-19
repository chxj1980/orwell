
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include "/home/nvidia/tegra_multimedia_api/include/nvbuf_utils.h"

const char *vertex_shader = R"glsl(
      #version 100
      attribute vec2 position;
      attribute vec2 tex_coord;
      varying vec2 tex_coord_var;

      uniform mat4 transform_matrix;

      void main()
      {

        tex_coord_var = tex_coord;
        gl_Position = transform_matrix * vec4(position, 0.0, 1.0);
      }
    )glsl";

const char *fragment_shader = R"glsl(
    #version 100
    precision mediump float;
    varying vec2 tex_coord_var;

    uniform sampler2D tex;

    void main()
    {
        vec4 tex_color = texture2D(tex, tex_coord_var);
        gl_FragColor = vec4(tex_color.r, tex_color.g, tex_color.b, 1.0); 
    }
    )glsl";

int main()
{
    int input_image_width = 64;
    int input_image_height = 64;
    unsigned char *input_image_data = malloc(input_image_width * input_image_height * 4 * sizeof(char));
    int output_image_width = 64;
    int output_image_height = 64;

    /* 4 color checkers */
    for (int i = 0; i < input_image_height; i++)
    {
        for (int j = 0; j < input_image_width; j++)
        {
            unsigned char R = 0;
            unsigned char G = 0;
            unsigned char B = 0;
            unsigned char A = 0;
            int imod = i % 2;
            int jmod = j % 2;
            if (!imod && !jmod)
                R = 255;
            if (!imod && jmod)
                G = 255;
            if (imod && !jmod)
                B = 255;
            if (imod && jmod)
            {
                R = 255;
                G = 255;
                B = 255;
            }
            *(char *)(input_image_data + (i * input_image_width * 4 + j * 4)) = R;
            *(char *)(input_image_data + (i * input_image_width * 4 + j * 4) + 1) = G;
            *(char *)(input_image_data + (i * input_image_width * 4 + j * 4) + 2) = B;
            *(char *)(input_image_data + (i * input_image_width * 4 + j * 4) + 3) = A;
        }
    }

    /* save the input image as a raw */
    FILE *fpin = fopen("in.raw", "wb+");
    fwrite(input_image_data, input_image_width * input_image_height * 4, 1, fpin);
    fclose(fpin);

    GLuint tex[2];
    GLuint fbo, vbo, ebo, v_shader, f_shader, shader_program;
    EGLDisplay egl_display;
    EGLSurface egl_surface;
    EGLContext egl_context;

    /* EGL context creation */
    EGLConfig egl_config;
    EGLint matching_configs;
    const EGLint config_attrib_list[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE};
    const EGLint pbuffer_attrib_list[] = {
        EGL_WIDTH,
        output_image_width,
        EGL_HEIGHT,
        output_image_height,
        EGL_NONE,
    };
    const EGLint context_attrib_list[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE};
    egl_display = eglGetDisplay((EGLNativeDisplayType)0);
    eglInitialize(egl_display, 0, 0);
    eglBindAPI(EGL_OPENGL_ES_API);
    eglChooseConfig(egl_display, config_attrib_list, &egl_config, 1, &matching_configs);
    egl_surface = eglCreatePbufferSurface(egl_display, egl_config, pbuffer_attrib_list);
    egl_context = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, context_attrib_list);
    eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);

    /* OpenGL init */
    glViewport(0, 0, output_image_width, output_image_height);

    glGenFramebuffers(1, &fbo);
    glGenTextures(2, tex);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    shader_program = glCreateProgram();

    v_shader = glCreateShader(GL_VERTEX_SHADER);
    f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(v_shader, 1, &vertex_shader, NULL);
    glShaderSource(f_shader, 1, &fragment_shader, NULL);
    glCompileShader(v_shader);
    glCompileShader(f_shader);
    glAttachShader(shader_program, v_shader);
    glAttachShader(shader_program, f_shader);

    /* OpenGL run */
    GLuint vertex_position, texture_position;
    GLint transform_matrix;

    const GLfloat vertices[] = {
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f};

    const GLuint elements[] = {
        0, 1, 2,
        2, 3, 0};

    GLfloat mat[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0};

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    glLinkProgram(shader_program);
    glUseProgram(shader_program);
    transform_matrix = glGetUniformLocation(shader_program, "transform_matrix");
    glUniformMatrix4fv(transform_matrix, 1, GL_FALSE, mat);
    vertex_position = glGetAttribLocation(shader_program, "position");
    glEnableVertexAttribArray(vertex_position);
    glVertexAttribPointer(vertex_position, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    texture_position = glGetAttribLocation(shader_program, "tex_coord");
    glEnableVertexAttribArray(texture_position);
    glVertexAttribPointer(texture_position, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, input_image_width, input_image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, input_image_data);
    glUniform1i(glGetUniformLocation(shader_program, "tex"), 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* EGLImage creation*/
    int dmabuf_fd;
    void *virtual_addr;
    EGLImageKHR egl_image;
    PFNGLEGLIMAGETARGETTEXTURE2DOESPROC EGLImageTargetTexture2DOES;

    /* NvBufferLayout_Pitch isn't working */
    NvBufferCreate(&dmabuf_fd, output_image_width, output_image_height, NvBufferLayout_BlockLinear, NvBufferColorFormat_ABGR32);
    NvBufferMemMap(dmabuf_fd, 0, NvBufferMem_Read_Write, &virtual_addr);
    egl_image = NvEGLImageFromFd(egl_display, dmabuf_fd);
    EGLImageTargetTexture2DOES = (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)eglGetProcAddress("glEGLImageTargetTexture2DOES");

    glBindTexture(GL_TEXTURE_2D, tex[1]);
    EGLImageTargetTexture2DOES(GL_TEXTURE_2D, (GLeglImageOES)egl_image);
    NvBufferMemSyncForDevice(dmabuf_fd, 0, virtual_addr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex[1], 0);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glFinish();
    NvBufferMemSyncForCpu(dmabuf_fd, 0, virtual_addr);

    /*
       * If I use glReadPixels here the output is OK
       * glReadPixels (0, 0, output_image_width, output_image_height, GL_RGBA, GL_UNSIGNED_BYTE, virtual_addr);
       * 
       */

    /* save the output image as a raw */
    FILE *fpout = fopen("out.raw", "w+");
    fwrite(virtual_addr, output_image_width * output_image_height * 8, 1, fpout);
    fclose(fpout);

    /* OpenGL stop */
    NvDestroyEGLImage(egl_display, egl_image);
    NvBufferMemUnMap(dmabuf_fd, 0, virtual_addr);
    NvBufferDestroy(dmabuf_fd);

    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(2, tex);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);
    glDeleteProgram(shader_program);
    eglDestroyContext(egl_display, egl_context);
    eglDestroySurface(egl_display, egl_surface);
    eglTerminate(egl_display);
    free(input_image_data);

    return 0;
}



    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    //externalTexture is the texture I'm gonna send the eglImage
    glGenTextures(1, &externalTexture);
    //bufferTexture is where we're going to render to
    glGenTextures(1, &frameBufferTexture);
    //Lines below create an empty texture with our image size (I'm assuming its RGBA, I don't know)
    glBindTexture(GL_TEXTURE_2D, externalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, decodedNvFrame->width, decodedNvFrame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    //Tells the shader which texture unit we're using
    glUniform1i(texLocation, 0);
    //Don't know what these are for, exactly
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Binds our frameBuffer because we're going to write to it
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);
    
    //Activate externalTexture because we want to put our hEglImage image there
    glBindTexture(GL_TEXTURE_2D, externalTexture);
    EGLImageKHR hEglImage;
    EGLSyncKHR eglSync;
    hEglImage = NvEGLImageFromFd(eglDisplay, decodedNvFrame->nvBuffer->planes[0].fd);
    //Puts hEglImage into externalTexture?
    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, hEglImage);
    glBindVertexArray(vertexArrayObject);
    //Draw to our frame buffer using the texture from externalTexture
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //Bind frameBufferTexture because we're going to read data from it with glReadPixels
    glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
    //-------------
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    //Just pick a small 512x512 image so we can see in the console if numbers appear 'random'
    glReadPixels(0, 0, 512, 512, GL_RGBA, GL_UNSIGNED_BYTE, r);

    for (int i = 0; i < 100; ++i)
    {
        printf("%i ", r[i]);
    }
    printf("\n");






        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glGenTextures(1, &externalTexture);
        glGenTextures(1, &frameBufferTexture);
        glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, decodedNvFrame->width, decodedNvFrame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glUniform1i(texLocation, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
	
		EGLImageKHR hEglImage;

		EGLSyncKHR eglSync;
		
		hEglImage = NvEGLImageFromFd(eglDisplay, decodedNvFrame->nvBuffer->planes[0].fd);
		if (!hEglImage)
			printf("Could not get EglImage from fd. Not rendering\n");
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);
		GLenum frameBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    	if (frameBufferStatus!=GL_FRAMEBUFFER_COMPLETE) {
			printf("frameBufferStatus problem!\n");
			abort();
		}

		glBindTexture(GL_TEXTURE_2D, externalTexture);
		glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, hEglImage);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glUniform1i(texLocation, 0);
		glBindVertexArray(vertexArrayObject);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glReadPixels(0, 0, 512, 512, GL_RGBA, GL_UNSIGNED_BYTE, r);

		for (int i = 0; i < 100; ++i)
		{
			printf("%i ", r[i]);
		}
		printf("\n");

		NvDestroyEGLImage(eglDisplay, hEglImage);




        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        assertOpenGLError("glBindFramebuffer");
        glGenTextures(1, &externalTexture);
        glGenTextures(1, &frameBufferTexture);
        glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, decodedNvFrame->width, decodedNvFrame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        assertOpenGLError("198");
        glUniform1i(texLocation, 0);
    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
	
		assertOpenGLError("220");
		EGLImageKHR hEglImage;

		EGLSyncKHR eglSync;
		int iErr;
		hEglImage = NvEGLImageFromFd(eglDisplay, decodedNvFrame->nvBuffer->planes[0].fd);
		if (!hEglImage)
			printf("Could not get EglImage from fd. Not rendering\n");
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);		
		glBindTexture(GL_TEXTURE_2D, externalTexture);
		glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, hEglImage);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glUniform1i(texLocation, 0);
		glBindVertexArray(vertexArrayObject);
		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		//-------------
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		GLenum frameBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    	if (frameBufferStatus!=GL_FRAMEBUFFER_COMPLETE) {
			printf("frameBufferStatus problem!\n");
			abort();
		}
		glReadPixels(0, 0, 512, 512, GL_RGBA, GL_UNSIGNED_BYTE, r);

		for (int i = 0; i < 100; ++i)
		{
			printf("%i ", r[i]);
		}
		printf("\n");

		NvDestroyEGLImage(eglDisplay, hEglImage);