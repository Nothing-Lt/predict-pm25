import urllib
import base64
import re
import os

strURL="http://www.tianqihoubao.com/aqi/"
strHTML=".html"
strFileName='TianQiHouBao'
strLabel='<table width="620px" border="0" class="b" cellpadding="1" cellspacing="1">'
value=[0000.000]*7

#get location 
def get_location():
    strlist=['city','country']
    if os.path.exists('Location') == False:
        os.system('python getlocation.py')
    reader=open('Location','r')
    for i in range(1,3):
        strlist[i-1]=reader.readline()
	strlist[i-1]=strlist[i-1].strip().lstrip().rstrip()
    return strlist

#http request download page
def get_TianQiHouBao(str_city):
	str_url=strURL+str_city+strHTML
	urllib.urlretrieve(str_url,strFileName)
	print str_url

def get_value_string(string):
	start_index=string.find('</td>')
	if 0 < start_index:
		match=re.compile('([\d]*\.*[\d]+)</td>')
		pattern=match.findall(string)
		if pattern:
			return pattern
		return '0'
	return '0'

#use pattern get value including :AQI,PM10,PM2.5,So2,Co2,O3,No2
def get_value():
	ele_number=0
	file_t=open(strFileName)
	for i in range(0,200):
		string=file_t.readline()
		if 0 < string.find(strLabel):
			for j in range(0,40):
				string=file_t.readline()
				if 0 < string.find('</tr>'):
					string = file_t.read()
					value_string = get_value_string(string)
					print len(value_string)
					print value_string
					ele_number=len(value_string)/7
					print ele_number
					for vIndex in range(0,ele_number):
						for vIndexi in range(0,7):
							value[vIndexi]=value[vIndexi]+float(value_string[vIndex*7+vIndexi])
						print value_string[vIndex*7+vIndexi]
					
					print value
					for vIndex in range(0,7):
						value[vIndex]=value[vIndex]/ele_number		
					return
		
#for test function
get_TianQiHouBao(get_location()[0])
get_value()
output=open('Data','w')
output.write(str(value[0])+'\n')
output.write(str(value[3])+'\n')
output.write(str(value[4])+'\n')
output.write(str(value[6])+'\n')
output.write(str(value[2])+'\n')
output.write(str(value[1])+'\n')
output.write(str(value[5])+'\n')
