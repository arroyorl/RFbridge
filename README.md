# RF Bridge
Bridge on RF 433 MHz, transmiting defibed codes/protocols, upon recepcion of defined codes/protocol

it uses rc-swtich library for receiving and transmit RF codes

HW based on ESP 8266 + CC1101 433 MHz RX/TX module

The program sytores a transmiting codes array with 5 codes (configurable), specifying for each one: code, protocol and pulse length. each of the TX code code has associated an index number (1 to 5). Also stores up to 10 RX codes, identified by code and protocol. for each RX entry it is defined wich TX index code will triger. Upon recepcition of a defined RX code, the program will transmit the associated TX code.

> [!NOTE]
> On first run, the program creates an access point with name "ESP-RF433" creating a web server accessible on http://192.168.4.1 which allow to configure SSID/password,node name and MQTT configuration. Once configured these parameters, the program connects to the configured WiFi and starts data capturing and submission. This AP can also been activated after a reset if the SETUP_PIN (GPIO 0) is down during a lapse of 5 seconds after reset.

Once done the first setup, there are several web pages which can be used for configuration, as well as RF codes trasmision.<br>
  **http://ip_address/setup**    Initial setu page, allows change WiFi and MQTT configuration<br>
  **http://ip_address/updateform**  form for define RX and TX codes, and association between them (wich TX code will be trigered upon reception of an defined RX code<br>
  **http://ip_address/listdata**    List all defined RX and TX codes<br>
  **http://ip_address/sendtxdata?txindex=nn**  Transmit the TX code with index nn<br>
  **http://ip_address/send?protocol=xx&code=cccccccc&length=nn** Transmit the decimal ccccccccc code with protocol xx and code length nn<br>
  **http://ip_address/send?protocol=xx&hexcode=hhhhhh&length=nn** same as previous with tyransmiting the hhhhhhh hexadecimal code<br>
  **http://ip_address/send?protocol=xx&bitcode=bbbbbbbbbbbbbbbbbbbb** transmit the bit code bbbbbbbbbbbbbbbbbbbb with protocol xx<br>
  **http://ip_address/txdata?txindex=nn&protocol=xx&pulselength=xx&hexcode=hhhhhh&datalength=nn&toggle=t** defines the transmit code on txindex, with protocol, hexa code and code length. toggle indiocates if this code is transmited on a hard reset<br>
  **http://ip_address/rxdata?rxindex=nn&protocol=xx&hexcode=hhhhhh&txindex=nn**  define the RX code on RX index, with protocol and hexa code, and associates it with TX code with index nn<br>
  **http://ip_address/txclear** Clear TX table<br>
  **http://ip_address/rxclear** Clear RX table<br>



