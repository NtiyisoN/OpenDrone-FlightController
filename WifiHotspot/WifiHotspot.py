import subprocess
import os.path
import xmltodict
import json
import os

def is_non_zero_file(fpath):
   return os.path.isfile(fpath) and os.path.getsize(fpath) > 0

def loadJSON():
    with open('/etc/accesspoint/accesspoint.json', 'w+') as f:
        f.write('{"ssid": "' + "OpenDrone" + '", "inet": "eth0", "wlan": "wlan0", "password": "' + "1234567890" + '", "netmask": "255.255.255.0", "ip": "192.168.1.254"}')
        f.seek(0)    

subprocess.call('sudo pyaccesspoint stop', shell=True)
if (os.path.isfile('/etc/accesspoint/accesspoint.json') == True):
    loadJSON()
    subprocess.call("sudo pyaccesspoint --config start", shell=True)
else:
    cmd = "sudo bash " + os.path.dirname(os.path.realpath(__file__)) + "/install.sh"
    subprocess.call(cmd, shell=True)
    loadJSON()
    subprocess.call("sudo pyaccesspoint --config start", shell=True)