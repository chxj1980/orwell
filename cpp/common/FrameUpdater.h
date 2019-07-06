#ifndef FRAMEUPDATER_H
#define FRAMEUPDATER_H

/*
Why create such a silly class? Well because I don't want XVideoWidget things
inside MediaStream because I don't want QT things there because I'm going to use
mediaStream on Android and iOS too. But I need to use the method updateData of 
XVideoWidget and possibly other widgets (for Android and iOS), so they must
implement this interface.
*/
class FrameUpdater {
   public:
      virtual void updateData(unsigned char**data, int frameWidth, int frameHeight)= 0;//https://stackoverflow.com/a/36909641/10116440
      void setFrameWidth(int width) {
        this->frameWidth = width;
      }

      void setFrameHeight(int height) {
         this->frameHeight = height;
      }
   int frameWidth = 0;
   int frameHeight = 0;
};
#endif // FRAMEUPDATER_H
