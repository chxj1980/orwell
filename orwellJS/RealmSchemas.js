//Cameras have just one ONVIF port, which returns many RTSP streams, which are CameraFlavor objects listed below
exports.Camera = {
    name: 'Camera',
    properties: {
      name: {type: 'string'},
      host: {type: 'string'},
      onvif: {type: 'int', default: 80},
      rtsp: {type: 'int?', default: 554},
      rtsp_auto: {type: 'bool?', default: true},
      flavors: {type:'list', objectType: 'CameraFlavor'},
      username: {type: 'string'},
      password: {type: 'string'},
      vpn_connection: {type:'list', objectType: 'VPNConnection'},
      use_https: {type: 'bool?', default: false}
    }
  };

exports.CameraFlavor = {
  name: 'CameraFlavor',
  properties: {
    rtsp_url: {type: 'string'},
    quality: {type: 'string', default: 'HIGHEST'},
    activated: {type: 'bool?', default: true}
  }
};

exports.VPNConnection = {
  name: 'VPNConnection',
  properties: {
    host: {type: 'string?'},
    port: {type: 'int?'},
    ovpn_file: {type: 'data'}
  }
};