i used digikey, so those are the links i'm providing. if you want to use another vender, go for it.

### buttons & switches
- 45x **White buttons** [6.2 x 6.2mm] [3-4mm tall] [[digikey]](https://www.digikey.com/en/products/detail/w%C3%BCrth-elektronik/431481031816/5209071)
  - Used for the button grid: .XX...XXX...
- 45x **Black buttons** [6.2 x 6.2mm] [3-4mm tall] [[digikey]](https://www.digikey.com/en/products/detail/cts-electrocomponents/222CMVBAR/5227985)
  - Used for the button grid: ...XXX...XX
- 3x **Vertical buttons** [[digikey]](https://www.digikey.com/en/products/detail/c-k/PTS645VH31-2-LFS/1146771)
  - Used for extras, eg decimal indicator, operation counter, store?
- 1x **Microswitch with lever** [[digikey]](https://www.digikey.com/en/products/detail/te-connectivity-alcoswitch-switches/SAJ101XBL0N74SDTPTQ/11312253)
  - Used for aesthetic clear
- 10x **Slide toggle switch** [SPDT/SPST] [[digikey]](https://www.digikey.com/en/products/detail/nidec-copal-electronics/CSS-1210TB/1124209)
  - Used for prevent-rollover switches
  - Under all screens except the right-most one.

### screens
*i used different common pin screens because that was what was available. don't copy me.
- 6x 7seg display [color 1] [12.4mm wide] [[digikey:blue]](https://www.digikey.com/en/products/detail/inolux/INND-SS56BAB/8343731) [blue]
  - Used for the screen: XXX...XXX..
- 5x 7seg display [color 2] [12.4mm wide] [[digikey:red]](https://www.digikey.com/en/products/detail/inolux/INND-SS56RCB/8343733)
  - Used for the screen: ...XXX...XX

### ICs
- 3x (2x4 bit) buffer [SN**74**AHCT**224N**] [[digikey]](https://www.digikey.com/en/products/detail/texas-instruments/SN74AHCT244N/375869)
  - We ran out of pins...
  - 1x - 8 bits for grid input
  - 1x - 8 bits for prevent-rollover-switches input
  - 1x - 4 bits for grid input, another 4 pins for prevent-rollover-switches input
- 2x 3:8 demuxer/decoder [SN**74**HC**138N**] [[digikey]](https://www.digikey.com/en/products/detail/texas-instruments/SN74HC138N/277221)
  - We ran out of pins...
  - 1x - Select buffer
  - 1x - Select number row [1,2,3,4,etc]
    - !! SNEAKY: Disable demuxer and look at 9's row
- 1x 4:10 demuxer/decoder [CD4028BE] [[digikey]](https://www.digikey.com/en/products/detail/texas-instruments/CD4028BE/67273)
  - We ran out of pins...
  - 1x - Screen select
    - !! SNEAKY: Disable demuxer and select the 11th screen
 - 3x 16pos DIP socket [[digikey]](https://www.digikey.com/en/products/detail/on-shore-technology-inc/SA163000/3313525)
   - **Make sure your specs match the ones for the demuxers** [if you use my links for both, all is good]
   - Used for demuxers
 - 3x 20pos DIP socket [[digikey]](https://www.digikey.com/en/products/detail/on-shore-technology-inc/SA203000/3313532)
   - **Make sure your specs match the ones for the buffers** [if you use my links for both, all is good]
   - Used for the buffers

### wires & boards
- big boi board [[digikey]](https://www.digikey.com/en/products/detail/vector-electronics/8016-1/416001)
  - For buttons & screen
  - Score & cut for the switches board
- smaller board [[digikey]](https://www.digikey.com/en/products/detail/twin-industries/8000-45/480489)
  - For logic stuff
- wires [i used custom colors] [these ones are 1ft long]
  - [yellow](https://www.digikey.com/en/products/detail/tubedepot/CW-20-SLD-YELLOW/10490365) - For number [1-9] select
  - [green](https://www.digikey.com/en/products/detail/tubedepot/CW-20-SLD-GREEN/10490081) - For digit [1,10,100,etc] select
  - [white](https://www.digikey.com/en/products/detail/tubedepot/CW-20-SLD-WHITE/10490268) - For switches
  - [blue](https://www.digikey.com/en/products/detail/tubedepot/CW-20-SLD-BLUE/10488998) - For screen data
  - [red](https://www.digikey.com/en/products/detail/tubedepot/CW-20-SLD-RED/10491003) - 5V
  - [black](https://www.digikey.com/en/products/detail/tubedepot/CW-20-SLD-BLACK/10489239) - GND

### extras
- resistors
  - 10x [160ohm](https://www.digikey.com/en/products/detail/stackpole-electronics-inc/CFM14JT160R/1742096) - For **my** red displays
  - 12x [100ohm](https://www.digikey.com/en/products/detail/stackpole-electronics-inc/CF14JT100R/1741261) - For **my** blue displays
- headers - I dont want to permanantely solder my wires to the board
  - 14x 8pos male [[digikey]](https://www.digikey.com/en/products/detail/sullins-connector-solutions/PREC008SBAN-M71RC/2774926)
  - 14x 8pos female [[digikey]](https://www.digikey.com/en/products/detail/adam-tech/SMC-1-08-1-GT/9830825)
  - 2x 20pos male [[digikey]](https://www.digikey.com/en/products/detail/adam-tech/PH1-20-UA/9830398) - Rpi Pico
  - 2x 20pos female [[digikey]](https://www.digikey.com/en/products/detail/sullins-connector-solutions/PPTC201LFBN-RC/810158) - Rpi Pico
- rpi pico [[not digikey]](https://www.raspberrypi.com/products/raspberry-pi-pico/)
  - The pin spacing here is 2.54mm, whereas the board is 1mm, so we will wire the female socket to the board by hand unfortunately
 - beautiful LED [[digikey]](https://www.digikey.com/en/products/detail/sunled/XSFRS23MBBA/4745859)
   - clear LED, since i'm not using a buzzer
 - beautiful power switch [[digikey]](https://www.digikey.com/en/products/detail/zf-electronics/SRB22A2FBRNN/446027)
   - idk, i like this one
 - 4xAAA or 4xAA battery holder [[digikey:AAA]](https://www.digikey.com/en/products/detail/keystone-electronics/2482/303827)
   - 6V is a bit high, but I think it's fine.
 
 its about $120, which is a lot, but whatever
 i also think [if i did my maths right] that every single pin on the rpi will be used
