i used digikey, so those are the links i'm providing. if you want to use another vender, go for it.

this is a project about aesthetics. i could certainly buy a regular pocket calculator, but that's honestly no fun. yes some parts are complete overkill, like the power switch, but i like it
## NOTICE
Everything **MUST** be compatible with 3.3V. The Raspberry Pi Pico does support 1.8V-5.5V input voltage, but all the GPIO pins are 3.3V, **NOT 5V**.

Be smart, do your research, unlike me ;] [i had to buy parts twice!]

## Already decided?
If you already want to build it and want to use DigiKey, you can create a upload a new list using [this file](https://github.com/VoxelPrismatic/comptelec/blob/main/PARTS.csv) [links to another file, don't right-click and save]. Already formatted so you don't have to deal with selecting column types

If not, you can continue and choose what you need

### buttons & switches
| Qty | Img | Description | Link | Notes |
|-|-|-|-|-|
| 45 | ![image](https://user-images.githubusercontent.com/45671764/157557460-20c1fa71-4f2b-4820-b2fc-7e60c91f6263.png) | White buttons | [[digikey]](https://www.digikey.com/en/products/detail/w%C3%BCrth-elektronik/431481031816/5209071) | - 6.2 x 6.2mm <br> - 3-4mm tall <br> - Used for the button grid: `.XX...XXX..`<br>**NOTE:** I recieved black buttons, so I'll paint them white. Feel free to buy 90 of the black buttons, it's cheaper |
| 45 | ![image](https://user-images.githubusercontent.com/45671764/157557504-e096d71c-fe9f-47dd-9a7c-3ae0a65d74d2.png) | Black buttons | [[digikey]](https://www.digikey.com/en/products/detail/cts-electrocomponents/222CMVBAR/5227985) | - 6.2 x 6.2mm <br> - 3-4mm tall <br> - Used for the button grid: `...XXX...XX` |
| 3 | ![image](https://user-images.githubusercontent.com/45671764/157557529-c10bf63b-dc34-47b7-9303-c134128868ab.png) | Vertical buttons | [[digikey]](https://www.digikey.com/en/products/detail/c-k/PTS645VH31-2-LFS/1146771) | Used for extras, eg:<br>- Decimal indicator<br>- Operation counter<br>- Store number? |
| 10 | ![image](https://user-images.githubusercontent.com/45671764/157557561-8fb936df-d806-455f-9d9b-03c17466a03d.png) | Slide toggle switch | [[digikey]](https://www.digikey.com/en/products/detail/nidec-copal-electronics/CSS-1210TB/1124209) | - SPDT/SPST<br>- Used for prevent rollover switches<br> - Under all screens except the rightmost one |
| 1 | ![image](https://user-images.githubusercontent.com/45671764/157560915-d2ffe916-7686-426f-8313-ade3d3097f1d.png) | Power switch | [[digikey]](https://www.digikey.com/en/products/detail/zf-electronics/SRB22A2FBRNN/446027) | I like this one, you can use a different switch if you want to |
| 1 | ![image](https://user-images.githubusercontent.com/45671764/157561008-d8617ae4-123d-4028-8d59-6c14e12331b6.png) | Microswitch with lever | [[digikey]](https://www.digikey.com/en/products/detail/te-connectivity-alcoswitch-switches/SAJ101XBL0N74SDTPTQ/11312253) | Aesthetic reset switch |


### screens
\*i used different common pin screens because that was what was available. don't copy me.
| Qty | Img | Description | Link | Notes |
|-|-|-|-|-|
| 6 | ![image](https://user-images.githubusercontent.com/45671764/157557677-bc4d02a3-f043-49cc-9de1-510243e90e5e.png) | 7seg display | [[digikey:blue]](https://www.digikey.com/en/products/detail/inolux/INND-SS56BAB/8343731) | - Color 1 [blue in my case]<br>- 12.4mm wide</br>- Used for the screen: `XXX...XXX..` |
| 5 | ![image](https://user-images.githubusercontent.com/45671764/157557699-6ddfea1d-ec7c-4c20-90aa-22f3146d18c1.png) | 7seg display | [[digikey:red]](https://www.digikey.com/en/products/detail/inolux/INND-SS56RCB/8343733) | - Color 2 [red in my case]<br>- 12.4mm wide</br>- Used for the screen: `...XXX...XX` |


### ICs
We ran out of pins on the RPi. Yes I could buy a second RPi and all would work well, but that means writing code to communicate between RPi's
| Qty | Img | Description | Link | Notes |
|-|-|-|-|-|
| 3 | ![image](https://user-images.githubusercontent.com/45671764/157557745-fe7488e2-2143-4dbf-ab58-569771ea22bf.png) | 2x4 bit buffer [74 LS 244] | [[digikey]](https://www.digikey.com/en/products/detail/texas-instruments/SN74LVC244AN/377479) | - 1x - 2x4 pins for grid input<br>- 1x - 2x4 pins for switches input<br>- 1x - 1x4 for grid, 1x4 for switches<br>**This is a 3.6V chip, do not use for other 5V projects** |
| 2 | ![image](https://user-images.githubusercontent.com/45671764/157557760-18c25cc4-2d87-4fcf-99d3-1931a1b1a5e9.png) | 3:8 demuxer/decoder [74 HC 138] | [[digikey]](https://www.digikey.com/en/products/detail/texas-instruments/SN74HC138N/277221) | - 1x - Select buffer<br> - 1x - Select number row<br><blockquote>SNEAKY: Disable demuxer and look at 9's row</blockquote> |
| 1 | ![image](https://user-images.githubusercontent.com/45671764/157557777-5e27652c-790e-4e88-af79-7cced4fabf77.png) | 4:10 demuxer/decoder [CD4028BE] | [[digikey]](https://www.digikey.com/en/products/detail/texas-instruments/CD4028BE/67273) | - 1x - Screen select<br><blockquote>SNEAKY: Disable demuxer and select the 11th screen</blockquote> |
| 1 | ![image](https://user-images.githubusercontent.com/45671764/157557805-2ae8f043-cdb0-465a-ae0b-f4b1d276a9de.png) | Hex inverter [*optional*] [74 HC 04] | [[digikey]](https://www.digikey.com/en/products/detail/toshiba-semiconductor-and-storage/TC74HC04APF/870457) | My blue displays are common cathode, so I need this to invert the high signal |
| 3 | ![image](https://user-images.githubusercontent.com/45671764/157557879-94cf018e-108c-485d-9074-fe3ce61d5270.png) | 16pos DIP socket | [[digikey]](https://www.digikey.com/en/products/detail/on-shore-technology-inc/SA163000/3313525) | - Used for demuxers<br>- **If you use a different chip than listed here, make sure the specs match** |
| 3 | ![image](https://user-images.githubusercontent.com/45671764/157557886-94084faf-c02c-4836-8685-757f8e9505fb.png) | 20pos DIP socket | [[digikey]](https://www.digikey.com/en/products/detail/on-shore-technology-inc/SA203000/3313532) | - Used for buffers<br>- **If you use a different chip than listed here, make sure the specs match** |
| 1 | ![image](https://user-images.githubusercontent.com/45671764/157557910-8b371920-82f3-4754-bed1-a4d2f9f65673.png) | 14pos DIP socket | [[digikey]](https://www.digikey.com/en/products/detail/on-shore-technology-inc/SA143000/3313545) | - Used for hex inverter<br>- **If you use a different chip than listed here, make sure the specs match** |

### wires & boards
\*wires are 1ft long
| Qty | Img | Description | Link | Notes |
|-|-|-|-|-|
| 1 | ![image](https://user-images.githubusercontent.com/45671764/157558123-d4447442-3787-44c6-a6d1-14fa50f1b519.png) | 6x6in perfboard | [[digikey]](https://www.digikey.com/en/products/detail/vector-electronics/8016-1/416001) | - For buttons & screen<br>- Score & cut for the switches board |
| 1 | ![image](https://user-images.githubusercontent.com/45671764/157558236-972a71c4-cc8c-4218-b7c5-6fada450f064.png) | Smaller perfboard | [[digikey]](https://www.digikey.com/en/products/detail/twin-industries/8000-45/480489) | - For logic stuff |
| 2 | ![image](https://user-images.githubusercontent.com/45671764/157558362-ef833c0f-413c-496d-8e72-30db1d0635ce.png) | Black wire | [[digikey]](https://www.digikey.com/en/products/detail/tubedepot/CW-20-SLD-BLACK/10489239) | Ground wires |
| 2 | ![image](https://user-images.githubusercontent.com/45671764/157559930-bbea0f3c-3f0e-45ee-b789-c3727c7984a1.png) | Red wires | [[digikey]](https://www.digikey.com/en/products/detail/tubedepot/CW-20-SLD-RED/10491003) | 3.3V wires |
| 4 | ![image](https://user-images.githubusercontent.com/45671764/157560112-f563bfe7-5908-4567-88c7-1bdf6e6e7142.png) | Blue wires | [[digikey]](https://www.digikey.com/en/products/detail/tubedepot/CW-20-SLD-BLUE/10488998) | Screen data |
| 3 | ![image](https://user-images.githubusercontent.com/45671764/157560219-9c6ff053-ead7-4530-90ac-f1b0daab86e7.png) | White wires | [[digikey]](https://www.digikey.com/en/products/detail/tubedepot/CW-20-SLD-WHITE/10490268) | Switches |
| 3 | ![image](https://user-images.githubusercontent.com/45671764/157560276-b6837b91-87a5-4b9f-9b98-29cb501ada34.png) | Yellow wires | [[digikey]](https://www.digikey.com/en/products/detail/tubedepot/CW-20-SLD-YELLOW/10490365) | For number [1-9] select |
| 3 | ![image](https://user-images.githubusercontent.com/45671764/157560357-a23d028f-469a-415c-9695-79dad1fb3788.png) | Green wires | [[digikey]](https://www.digikey.com/en/products/detail/tubedepot/CW-20-SLD-GREEN/10490081) | For digit [1s, 10s, 100s, etc] select |

### extras
| Qty | Img | Description | Link | Notes |
|-|-|-|-|-|
| 10 | ![image](https://user-images.githubusercontent.com/45671764/157560518-04dcf8a7-d900-4ca4-843c-cb5487f0bc70.png) | 75Ω resistor | [[digikey]](https://www.digikey.com/en/products/detail/stackpole-electronics-inc/CFM12JT75R0/1742006) | - For **my** red displays<br>- If you use different displays, use a different resistor |
| 12 | ![image](https://user-images.githubusercontent.com/45671764/157560686-ddff0a14-b2c0-48e3-9039-612ff14773de.png) | 8.2Ω resistor | [[digikey]](https://www.digikey.com/en/products/detail/stackpole-electronics-inc/CFM12JT8R20/2617275) | - For **my** blue displays<br>- If you use different displays, use a different resistor |
| 14 | ![image](https://user-images.githubusercontent.com/45671764/157561369-77a5953b-2768-4e28-8534-fbb58d8e1a70.png) | 8pos male connector | [[digikey]](https://www.digikey.com/en/products/detail/sullins-connector-solutions/PREC008SBAN-M71RC/2774926) | Makeshift ribbon cable<br>I don't want to solder wires directly between boards |
| 14 | ![image](https://user-images.githubusercontent.com/45671764/157561487-96cefc12-6e2e-4f2c-8d74-091a60bbe5df.png) | 8pos female connector | [[digikey]](https://www.digikey.com/en/products/detail/adam-tech/SMC-1-08-1-GT/9830825) | ^ |
| 2 | ![image](https://user-images.githubusercontent.com/45671764/157561554-79ce6610-e477-4d55-92ed-371e88e7c064.png) | 14pos male connector | [[digikey]](https://www.digikey.com/en/products/detail/adam-tech/PH1-20-UA/9830398) | For rpi, I don't want to solder directly to the board<blockquote>Included with RPi Pico from **Adafruit**</blockquote> |
| 2 | ![image](https://user-images.githubusercontent.com/45671764/157561667-df3ecfa8-8b14-417a-b8d7-59206a594c19.png) | 14pos female connector | [[digikey]](https://www.digikey.com/en/products/detail/sullins-connector-solutions/PPTC201LFBN-RC/810158) | For rpi, I don't want to solder directly to the board<blockquote>**Adafruit** does not include these</blockquote> |
| 1 | ![image](https://user-images.githubusercontent.com/45671764/157561921-c46e6dff-486a-47a1-8399-5a4046cc4db1.png) | Raspberry Pi __PICO__ | [[digikey]](https://www.digikey.com/en/products/detail/raspberry-pi/SC0915/13624793) | Main controller |
| 1 | ![image](https://user-images.githubusercontent.com/45671764/157561986-0dbd8da0-9b0a-416e-afb5-8d0dd6d64448.png) | Ice cube Ice blue LED | [[digikey]](https://www.digikey.com/en/products/detail/sunled/XSFRS23MBBA/4745859) | - Reset LED<br>- This specific LED is purely aesthetic, but you should still buy an LED |
| 1 | ![image](https://user-images.githubusercontent.com/45671764/157562085-40586a73-af80-4c6c-a610-53cd20849324.png) | 3 cell AA/AAA holder | [[digikey:AAA]](https://www.digikey.com/en/products/detail/keystone-electronics/2480/303825)<br>[[digikey:AA]](https://www.digikey.com/en/products/detail/keystone-electronics/2465/303814) | **NOTE:** I used the __AAA__ holder [top link], so my enclosure will reflect that<br>**NOTE:** The RPi Pico WILL BE DAMAGED if you use a 4-cell holder. Operating voltage is 1.8V ~ 5.5V<br>**DO NOT PURCHASE A 4 CELL HOLDER WITHOUT PROPER VOLTAGE REGULATION** |
 
Altogether costs $121.12 in parts alone [as of Mar09,2022]. You could buy a graphing calculator for an additional $25, but if you made it this far, I think you agree that it's well worth the experience.

If this project is popular enough, I may design a PCB so perfboards aren't in the picture anymore.

i also think [if i did my maths right] that every single pin on the rpi will be used
