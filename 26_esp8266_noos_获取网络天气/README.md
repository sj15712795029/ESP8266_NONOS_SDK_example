目前我是访问百度云市场的“全国天气预报”
域名是：gwgp-n6uzuwmjrou.n.bdcloudapi.com
GET请求字符串是：GET  /weather/query?cityid=%283 HTTP/1.1\r\nHost: gwgp-n6uzuwmjrou.n.bdcloudapi.com\r\nX-Bce-Signature: AppCode/4b1270235a6a4399a777dbbe3f961741\r\n\r\n
</br>文档参考：https://cloud.baidu.com/doc/Market/s/fjx18s856
----------- http ------------------------
GET  /weather/query?cityid=283 HTTP/1.1
Host: gwgp-n6uzuwmjrou.n.bdcloudapi.com
X-Bce-Signature: AppCode/4b1270235a6a4399a777dbbe3f961741

----------- http ------------------------
▒▒▒▒▒▒ݳɹ▒▒▒
----------- http recv data len 254-----------
HTTP/1.1 200 OK
transfer-encoding: chunked
Server: nginx/1.16.0
Date: Thu, 06 Feb 2020 14:21:38 GMT
Content-Type: text/html; charset=utf-8
X-Powered-By: PHP/7.1.29
X-Bce-Request-Id: 27cab1ae-678a-4397-a7f9-796cf7c2998a
X-Bce-Gateway-Region: BJ


----------- http recv data end----------

----------- http recv data len 1440-----------
1439
{"status":0,"msg":"ok","result":{"city":"青岛","cityid":283,"citycode":"101120201","date":"2020-02-06","week":"星期四","weather":"多云","temp":"0","temphigh":"0","templow":"-1","img":"1","humidity":"68","pressure":"1023","windspeed":"2.4","winddirect":"东南风","windpower":"2级","updatetime":"2020-02-06 21:43:00","index":[{"iname":"空调指数","ivalue":"开启制暖空调","detail":"您将感到有些冷，可以适当开启制暖空调调节室内温度，以免着凉感冒。"},{"iname":"运动指数","ivalue":"较不宜","detail":"天气较好，但考虑天气寒冷，风力较强，推荐您进行室内运动，若在户外运动请注意保暖并做好准备活动。"},{"iname":"紫外线指数","ivalue":"最弱","detail":"属弱紫外线辐射天气，无需特别防护。若长期在户外，建议涂擦SPF在8-12之间的防晒护肤品。"},{"iname":"感冒指数","ivalue":"极易发","detail":"寒冷潮湿，易发生感冒，请注意适当增加衣服，加强自我防护避免感冒。"},{"iname":"洗车指数","ivalue":"适宜","detail":"适宜洗车，未来持续两天无雨天气较好，适合擦洗汽车，蓝天白云、风和日丽将伴您的车子连日洁净。"},{"iname":"空气污染扩散指数","ivalue":"良","detail":"气象条件有利于空气污染物稀释、扩散和清除。"},{"iname":"穿衣指数","ivalue":"寒冷","detail":"天气寒冷，建议着厚羽绒服
----------- http recv data end----------

----------- http recv data len 1440-----------
、毛皮大衣加厚毛衣等隆冬服装。年老体弱者尤其要注意保暖防冻。"}],"aqi":{"so2":"7","so224":"","no2":"28","no224":"","co":"0.600","co24":"","o3":"44","o38":"","o324":"","pm10":"28","pm1024":"","pm2_5":"19","pm2_524":"","iso2":"2","ino2":"14","ico":"6","io3":"13","io38":"","ipm10":"28","ipm2_5":"27","aqi":"28","primarypollutant":"PM10","quality":"优","timepoint":"2020-02-06 21:00:00","aqiinfo":{"level":"一级","color":"#00e400","affect":"空气质量令人满意，基本无空气污染","measure":"各类人群可正常活动"}},"daily":[{"date":"2020-02-06","week":"星期四","sunrise":"06:54","sunset":"17:30","night":{"weather":"多云","templow":"-1","img":"1","winddirect":"南风","windpower":"4级"},"day":{"weather":"阴","temphigh":"0","img":"2","winddirect":"西风","windpower":"3级"}},{"date":"2020-02-07","week":"星期五","sunrise":"06:54","sunset":"17:31","night":{"weather":"多云","templow":"0","img":"1","winddirect":"南风","windpower":"4级"},"day":{"weather":"多云","temphigh":"5","img":"1","winddirect":"南风","windpower":"4级"}},{"date":"2020-02-08","week":"星期六","sunrise":"06:53","sunset":"17:32","night":{"weather":"晴","templow":"2","img":"0","winddirect":"北风","windpower":"4级"},"day":{"weather":"晴","temphigh":"8","img":"0","winddirect":"微风","windpower":"4级"}},{"date":"2020-02-09","week":"星期日","sunrise":"06:52","sunset":"17:33","night":{"wea
----------- http recv data end----------

----------- http recv data len 1440-----------
ther":"晴","templow":"3","img":"0","winddirect":"西南风","windpower":"4级"},"day":{"weather":"晴","temphigh":"9","img":"0","winddirect":"西南风","windpower":"4级"}},{"date":"2020-02-10","week":"星期一","sunrise":"06:51","sunset":"17:34","night":{"weather":"多云","templow":"4","img":"1","winddirect":"西南风","windpower":"4级"},"day":{"weather":"晴","temphigh":"9","img":"0","winddirect":"南风","windpower":"4级"}},{"date":"2020-02-11","week":"星期二","sunrise":"06:50","sunset":"17:35","night":{"weather":"小雨","templow":"2","img":"7","winddirect":"东南风","windpower":"4级"},"day":{"weather":"阴","temphigh":"7","img":"2","winddirect":"南风","windpower":"4级"}},{"date":"2020-02-12","week":"星期三","sunrise":"06:49","sunset":"17:36","night":{"weather":"多云","templow":"2","img":"1","winddirect":"西北风","windpower":"4级"},"day":{"weather":"小雨","temphigh":"8","img":"7","winddirect":"西北风","windpower":"4级"}}],"hourly":[{"time":"23:00","weather":"阴","temp":"0","img":"2"},{"time":"00:00","weather":"阴","temp":"0","img":"2"},{"time":"01:00","weather":"阴","temp":"0","img":"2"},{"time":"02:00","weather":"阴","temp":"0","img":"2"},{"time":"03:00","weather":"阴","temp":"0","img":"2"},{"time":"04:00","weather":"阴","temp":"0","img":"2"},{"time":"05:00","weather":"阴","temp":"0","img":"2"},{"time":"06:00","weather":"阴","temp":"0","img":"2"},{"time":"07:00","weather":"
----------- http recv data end----------

----------- http recv data len 865-----------
阴","temp":"0","img":"2"},{"time":"08:00","weather":"阴","temp":"1","img":"2"},{"time":"09:00","weather":"阴","temp":"1","img":"2"},{"time":"10:00","weather":"阴","temp":"2","img":"2"},{"time":"11:00","weather":"多云","temp":"3","img":"1"},{"time":"12:00","weather":"阴","temp":"3","img":"2"},{"time":"13:00","weather":"阴","temp":"4","img":"2"},{"time":"14:00","weather":"多云","temp":"4","img":"1"},{"time":"15:00","weather":"多云","temp":"4","img":"1"},{"time":"16:00","weather":"阴","temp":"4","img":"2"},{"time":"17:00","weather":"阴","temp":"4","img":"2"},{"time":"18:00","weather":"多云","temp":"4","img":"1"},{"time":"19:00","weather":"多云","temp":"3","img":"1"},{"time":"20:00","weather":"多云","temp":"3","img":"1"},{"time":"21:00","weather":"多云","temp":"3","img":"1"},{"time":"22:00","weather":"多云","temp":"3","img":"1"}]}}

----------- http recv data end----------

----------- http recv data len 5-----------
0


----------- http recv data end----------




还有心知天齐get字符串为：
GET /v3/weather/daily.json?key=rrpd2zmqkpwlsckt&location=guangzhou&language=en&unit=c&start=0&days=3 HTTP/1.1
Content-Type: text/html;charset=utf-8
Accept: */*
Host: api.seniverse.com
Connection: Keep-Alive

</br>心知天气参考：https://docs.seniverse.com/api/start/key.html
两个对比是：如果用百度的，那么需要有编解码，如果心知，那么不需要

