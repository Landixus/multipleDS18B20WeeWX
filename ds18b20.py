import syslog
import urllib2
import requests
import html2text
import weewx
from weewx.wxengine import StdService

class ds18b20(StdService):
    def __init__(self, engine, config_dict):
        super(ds18b20, self).__init__(engine, config_dict)
        d = config_dict.get('PondService', {})
        url = "http://192.168.0.70"
        page = urllib2.urlopen(url)
        html_content = page.read()
        rendered_content = html2text.html2text(html_content)
        file = open('/home/weewx/public_html/data/ds18b20.txt', 'w')
        file.write(rendered_content)
        file.close()
        self.filename = d.get('filename', '/home/weewx/public_html/data/ds18b20.txt')
        syslog.syslog(syslog.LOG_INFO, "ds18b20: using %s" % self.filename)
        self.bind(weewx.NEW_ARCHIVE_RECORD, self.read_file)

    def read_file(self, event):
        try:
            with open(self.filename) as f:
                #value = f.read()
                line = f.readline()
                value = line.split(',')
            syslog.syslog(syslog.LOG_DEBUG, "ds18b20: found value of %s" % value)
            event.record['soilTemp1'] = float(value[0])
            event.record['soilTemp2'] = float(value[1])
            event.record['soilTemp3'] = float(value[2])
            event.record['soilTemp4'] = float(value[3])
        except Exception, e:
            syslog.syslog(syslog.LOG_ERR, "ds18b20: cannot read value: %s" % e)
