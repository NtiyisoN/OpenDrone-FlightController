import subprocess
import os.path
import xmltodict
import json

def loadJSON():
    with open('/home/pi/projects/OpenDrone_FC/CONFIG.xml') as fd:
        doc = xmltodict.parse(fd.read())

    if 'accesspointName' in doc['opendrone'] or 'accesspointPassword' in doc['opendrone']:
        json_data=open('/etc/accesspoint/accesspoint.json').read()
        j = json.loads(json_data)
        name=''
        password=''
        if 'accesspointName' in doc['opendrone']:
            name = doc['opendrone']['accesspointName']
            if name==None:
                name = j['ssid']
            

        if 'accesspointPassword' in doc['opendrone']:
            password = doc['opendrone']['accesspointPassword']
            if password==None:
                password = j['password']

        with open('/etc/accesspoint/accesspoint.json', 'w+') as f:
                f.write('{"ssid": "'+str(name)+'", "inet": null, "wlan": "wlan0", "password": "'+str(password)+'", "netmask": "255.255.255.0", "ip": "192.168.1.254"}')
                f.seek(0)
    subprocess.call("sudo sudo pyaccesspoint --config start", shell=True)

if os.path.isfile('/etc/accesspoint/accesspoint.json'):
    print("started hotspot")
    loadJSON()
else:
    subprocess.call("sudo chmod u+x ./install.sh", shell=True)
    subprocess.call("sudo bash ./install.sh", shell=True)
    loadJSON()