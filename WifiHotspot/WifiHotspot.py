import subprocess
import os.path
import xmltodict
import json
import os

def is_non_zero_file(fpath):
   return os.path.isfile(fpath) and os.path.getsize(fpath) > 0

def loadJSON():
    if is_non_zero_file('/home/pi/projects/OpenDrone_FC/CONFIG.xml'):
        with open('/home/pi/projects/OpenDrone_FC/CONFIG.xml') as fd:
            doc = xmltodict.parse(fd.read())

            if 'accesspointName' in doc['opendrone'] or 'accesspointPassword' in doc['opendrone']:
                if os.path.isfile('/etc/accesspoint/accesspoint.json') is False:
                    subprocess.call('sudo touch /etc/accesspoint/accesspoint.json', shell=True)

                json_data = open('/etc/accesspoint/accesspoint.json').read()

                j = json.loads(json_data)
                name = ''
                enterCode = ''
                if 'accesspointName' in doc['opendrone']:
                    name = doc['opendrone']['accesspointName']
                    if name is None:
                        name = j['ssid']
            

                if 'accesspointPassword' in doc['opendrone']:
                    enterCode = doc['opendrone']['accesspointPassword']
                    if enterCode is None:
                        enterCode = j['password']

                with open('/etc/accesspoint/accesspoint.json', 'w+') as f:
                        f.write('{"ssid": "' + str(name) + '", "inet": "eth0", "wlan": "wlan0", "password": "' + str(enterCode) + '", "netmask": "255.255.255.0", "ip": "192.168.1.254"}')
                        f.seek(0)
    else:
        with open('/etc/accesspoint/accesspoint.json', 'w+') as f:
               f.write('{"ssid": "' + "OpenDrone" + '", "inet": "eth0", "wlan": "wlan0", "password": "' + "1234567890" + '", "netmask": "255.255.255.0", "ip": "192.168.1.254"}')
               f.seek(0)    

    

subprocess.call('sudo pyaccesspoint stop')
if os.path.isfile('/etc/accesspoint/accesspoint.json'):
    loadJSON()
    subprocess.call("sudo pyaccesspoint --config start", shell=True)
else:
    cmd = "sudo bash " + os.path.dirname(os.path.realpath(__file__)) + "/install.sh"
    subprocess.call(cmd, shell=True)
    loadJSON()
    subprocess.call("sudo pyaccesspoint --config start", shell=True)