import urllib

strURL='http://192.168.1.112:8080/PM25PredictServer/getimport?city='

strCity='Beijing'
file_t=open('Location')
strCityURL=strURL+file_t.readline()
print strCityURL
urllib.urlretrieve(strCityURL,'Important')
