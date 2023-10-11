# Moode-servo-volume

Not only can you control the volume of moode with a physical knob.  If you change the volume on Moode's interface the physical knob will move the appropriate position. This is accomplised with a Datan S1123 micro servo with analog feedback (A fourth wire that can be read with analog input on an arduino.
The Arduino communicates with the raspberry pi over USB.
## Circuit
Build Circuit on Arduino Im using the colors on my servo. You may have to use different pin numbers.  Change ino file accordingly.
* Connect white wire on servo to A0 analog In on arduino
* Connect orange wire to PWM pin 9
* Connect red wire to 5v
* Connect brown wire to ground.
* plug the arduino into the usb port of the raspberry pi.

## Installing
* Upload the ino file to an arduino board.  Any should do.
* Create a moode-servo-volume directory on your moode player
* Place knob.py in this moode-servo-volume directory
* move knob.service /etc/systemd/system
* In /opt/moode-servo-volume directory run 'sudo pip install -r requirements.txt'

You should plug into the arduino before you start the knob service.
