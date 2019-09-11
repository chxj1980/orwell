//https://github.com/agsh/onvif
//npm install onvif

var
  http = require('http'),
  Cam = require('onvif').Cam;

new Cam({
  hostname: '192.168.0.118',
  username: 'admin',
  password: '19929394..',
  port: 80
}, function(err) {
  if (err) {
      console.log('Connection Failed');
      return;
  }
  this.getStreamUri({protocol:'RTSP'}, function(err, stream) {
    console.log(stream.uri);
  });
});
