// html->string 변환사이트
// http://davidjwatts.com/youtube/esp8266/esp-convertHTM.html#
// https://tomeko.net/online_tools/cpp_text_escape.php?lang=en
String webHead="<html> <head> <meta name='viewport' content='width=device-width, initial-scale=1.0'/> <meta http-equiv='Content-Type' content='text/html;charset=utf-8' /> <style> body { background: #eab0dc; font-family: \\\"Lato\\\", sans-serif; } .button { border: none; color: white; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; } .button-on {border-radius: 100%; background-color: #4CAF50;} .button-off {border-radius: 100%;background-color: #707070;} .button-ledon {border-radius: 100%; padding: 10px; font-size: 8px; margin: 0px 0px; background-color: #ff4500;} .button-ledoff {border-radius: 100%; padding: 10px; font-size: 8px; background-color: #707070;} </style> </head> <body> <h3 style='color:SteelBlue;'>&emsp;김동일유튜브 <a href='http://i2r.link' style='color:SteelBlue;'>http://i2r.link</a> </h3>";
String webTail="\n<p><a href='/wifi'>네트웍공유기변경-누른후 와이파설정부터 다시하세요</a></p></body>\n</html>";
String webHeadM="<html> <head> <meta http-equiv='refresh' content='2'/> <meta name='viewport' content='width=device-width, initial-scale=1.0'/> <meta http-equiv='Content-Type' content='text/html;charset=utf-8' /> <style> body { background: #eab0dc; font-family: \\\"Lato\\\", sans-serif; } .button { border: none; color: white; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; } .button-on {border-radius: 100%; background-color: #4CAF50;} .button-off {border-radius: 100%;background-color: #707070;} .button-ledon {border-radius: 100%; padding: 10px; font-size: 8px; margin: 0px 0px; background-color: #ff4500;} .button-ledoff {border-radius: 100%; padding: 10px; font-size: 8px; background-color: #707070;} </style> </head> <body> <h3 style='color:SteelBlue;'>&emsp;김동일유튜브 <a href='http://i2r.link' style='color:SteelBlue;'>http://i2r.link</a> </h3>";

void handleRoot() {
  String s=""; 

  s+="<br>출력<table style='width:100%'>";
  for(int i=0;i<4;i++) {
    s+="<tr>";
    s+="<form action='/on'>";
    s+="<input type='hidden' name='act' value='"+(String)i+"'>";
    if(p4[i]==1) 
      s+="<button type='submit' name='value' value='0' class='button button-on' >4"+(String)i+"</button></a>"; //off
    else 
      s+="<button type='submit' name='value' value='1' class='button button-off' >4"+(String)i+"</button></a>"; //on
    s+="</form>";
    s+="</tr>";
  }

  String sm="";
  //sm+="<div style='background-color:LightSalmon;width: 300px;'>";
  sm+="<br>입력";
  for(int i=0;i<8;i++) {
    //Serial.println(p0[i]);
    if(p0[i]==1) 
      sm+="<button class='button button-ledon' >"+(String)i+"</button></a>"; 
    else 
      sm+="<button class='button button-ledoff' >"+(String)i+"</button></a>"; 
  }
  //sm+="</div>";

  sm+="<br><form action='/on'>모니터링";
  sm+="<input type='hidden' name='act' value='4'>";
  if(monit==1) 
    sm+="<button type='submit' name='value' value='0' class='button button-on' >on</button></a>"; //off
  else 
    sm+="<button type='submit' name='value' value='1' class='button button-off' >off</button></a>"; //on
  sm+="</form>";

  sm+="<br>통신에러";
  if(errorPlc==1) 
    sm+="<button class='button button-ledon' ></button></a>";
  else 
    sm+="<button class='button button-ledoff' ></button></a>"; 

  sm+="&emsp;P4의 모니터링 : ";
  sm+=sP4;

  sm+="<br><br>AP & IP :&emsp;"+sChipId+"&emsp;"+WiFi.localIP().toString();
  if(monit==1)
    server.send(200, "text/html", webHeadM+s+sm+webTail);
  else
    server.send(200, "text/html", webHead+s+sm+webTail);
}

void handleWifi() {
  WiFiManager wm;
  wm.resetSettings();
  wm.resetSettings();
  ESP.reset();
}

void handleMonit() {
  if(monit==1)
    monit=0;
  else
    monit=1;
  GoHome();
}

void handleOn() {
  int act=server.arg("act").toInt();
  int value=server.arg("value").toInt();

  Serial.println("~~~~~~~~~~~~~~~~~~~");
  Serial.println(act);
  Serial.println(value);
  if(act<4) {
    p4[act]=value;
    sendTextToPlc();
  }
  else if(act==4) {
    monit=value;
  }
  GoHome();
}

void GoHome() {
  String s,ipS;
  ipS=toStringIp(WiFi.localIP());
  s="<meta http-equiv='refresh' content=\"0;url='http://"+ipS+"/'\">";
  server.send(200, "text/html", s);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleScan() {
  String s="",sp0="",s4="";
  sp0=(String)p0[0]+(String)p0[1]+(String)p0[2]+(String)p0[3]+(String)p0[4]+(String)p0[5];
  s4=(String)p4[0]+(String)p4[1]+(String)p4[2]+(String)p4[3];
  s="{\"chipId\":\""+sChipId+"\",\"p0\":\""+sp0+"\", \"p4\":\""+s4+"\",\"monit\":\""+monit+"\"}";
  Serial.println(s);
  server.send(200, "text/html", s);
}
