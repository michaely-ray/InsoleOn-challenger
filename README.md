# InsoleOn-challenger
Código implementado para o Desafio proposto pela equipe de desenvolvimento InsoleOn.

1°) Configurar um broker para comunicação MQTT. Pode-se utilizar qualquer broker público, por exemplo: HiveMQ, eclipse... 
2°) Instalar um aplicativo MQTT.fx.
3°) Desenvolver um código utilizando o ESP-IDF, com a ESP32. O código deve funcionar da seguinte forma, quando o JSON, {“action”: 1}, for enviado ao tópico “/led” o led da placa deve acender, e quando o JSON, {“action”: 0}, for enviado o led deve apagar. O comando deve ser enviado pelo MQTT.fx. A biblioteca CJSON deve ser utilizada para validar a mensagem, caso a mensagem esteja incorreta deve ser enviado um arquivo JSON para o tópico “/erro” com o endereço MAC do dispositivo, por exemplo: {“mac”:”ac:67:b2:95:fc:76”}.
4°) O código deve ser desenvolvido e compartilhado utilizando o github.

Links de apoio:
https://www.arduinoecia.com.br/enviando-mensagens-mqtt-modulo-esp32-wifi/
https://www.newtoncbraga.com.br/index.php/microcontroladores/54-dicas-de-pic/16748-primeiro-programa-phyton-com-mqtt-na-raspberry-pi-introducao.html#:~:text=Instala%C3%A7%C3%A3o%20da%20biblioteca%20Paho%2DMQTT%20para%20Python&text=Esta%20biblioteca%20%C3%A9%20um%20projeto,como%20Java%2C%20C%20e%20Python.
https://github.com/Mair/esp32-course
https://cjson.docsforge.com/master/api/
