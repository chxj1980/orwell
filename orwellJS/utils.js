//Writes some cameras to realm for testing
ips = ['192.168.0.101', '192.168.0.102', '192.168.0.103', '192.168.0.104', '192.168.0.105', '192.168.0.106', '192.168.0.107'];

export default function createSomeCameras(realm) {
    realm.write(() => {
        var cameras = [];
        var i=1;
        for (const ip of ips){
            cameras.push(
                {
                    name: "Cam " + i.toString(),
                    host: ip,
                    onvif: 10080,
                    flavors: [
                        {
                            rtsp_url: 'rtsp://admin:19929394@' + ip + ':10554/tcp/av0_1',
                            quality: 'LOWEST',
                        },
                        {
                            rtsp_url: 'rtsp://admin:19929394@' + ip + ':10554/tcp/av0_0',
                            quality: 'HIGHEST',
                        }
                    ],
                    username: 'admin',
                    password: '19929394',
                    vpn_connection: [{ovpn_file: new ArrayBuffer(0)}]
                }
            )
            i+=1;
        }
        for (const camera of cameras){
          realm.create('Camera', camera);
        }
    });
}