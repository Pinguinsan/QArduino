# Script file for scomm
# testscript.sc
#
# DelaySeconds(), DelayMilliseconds(), and DelayMicroseconds() all accept a single
# parameter, and will delay for the amount of provided time

# The Write() command takes a single quotation mark delimited string that wil be written to the
# connected serial port

# The Loop() command takes a single argument and will loop all of the commands in the curly bracket delimiters
# If Loop() is not given an integer, it will loop forever

# The FlushRX() command flushes the Serial Port receive line
# The FlushTX() command flushes the Serial Port transit line
# The FlushRXTX() and FlushTXRX() commands both flush the Serial Port receive and transmit lines

# Any line began with a pound sign will be ignored

# Any line that has invalid syntax will be ignored, and a warning will be output to stdout

Loop() {
    Write("{addshortwd:3:0:failsafe:30:1:31:1:32:1:33:1}")
    DelayMilliseconds(25)
    Read()
    FlushRXTX()
    Write("{dwrite:30:0}")
    DelayMilliseconds(25)
    Read()
    FlushTXRX()
    Write("{dwrite:31:0}")
    DelayMilliseconds(25)
    Read()
    FlushTXRX()
    Write("{dwrite:32:0}")
    DelayMilliseconds(25)
    Read()
    FlushTXRX()
    Write("{dwrite:33:0}")
    DelayMilliseconds(25)
    Read()
    FlushTXRX()
    Write("{canlup:1}")
    DelayMilliseconds(1000)
    FlushTXRX()
    Write("{caninit}")
    DelayMilliseconds(1000)
    FlushTXRX()
    
    Loop(5) {    
        #Top Left Inidicator
        Write("{canwrite:0x3B3:0x40:0x00:0x00:0x12:0x00:0x00:0x00:0x00}")
        DelayMilliseconds(25)
        Write("{canwrite:0x263:0x02:0x00:0x00:0x00:0x00:0x00:0x00:0x00}")
        DelayMilliseconds(25)
        Write("{canwrite:0x44C:0x40:0x00:0x91:0x00:0x00:0x00:0x00:0x00}")
        Read()
        DelayMilliseconds(1000)
        FlushRXTX()    

        #Top Right Inidicator
        Write("{canwrite:0x3B3:0x40:0x00:0x00:0x12:0x00:0x00:0x00:0x00}")
        DelayMilliseconds(25)
        Write("{canwrite:0x263:0x00:0x00:0x00:0x00:0x00:0x00:0x00:0x00}")
        DelayMilliseconds(25)
        Write("{canwrite:0x44C:0x00:0x04:0x91:0x00:0x00:0x00:0x00:0x00}")
        Read()
        DelayMilliseconds(1000)
        FlushRXTX()    

        #Bottom Left Inidicator
        Write("{canwrite:0x3B3:0x40:0x00:0x00:0x12:0x00:0x00:0x00:0x00}")
        DelayMilliseconds(25)
        Write("{canwrite:0x263:0x00:0x00:0x01:0x00:0x00:0x00:0x00:0x00}")
        DelayMilliseconds(25)
        Write("{canwrite:0x44C:0x00:0x00:0x91:0x00:0x00:0x00:0x00:0x00}")
        Read()
        DelayMilliseconds(1000)
        FlushRXTX()    

        #Bottom Right Inidicator
        Write("{canwrite:0x3B3:0x40:0x00:0x00:0x12:0x00:0x00:0x00:0x00}")
        DelayMilliseconds(25)
        Write("{canwrite:0x263:0x00:0x00:0x00:0x00:0x04:0x00:0x00:0x00}")
        DelayMilliseconds(25)
        Write("{canwrite:0x44C:0x00:0x00:0x91:0x00:0x00:0x00:0x00:0x00}")
        Read()
        DelayMilliseconds(1000)
        FlushRXTX()    

        #All Indicators
        Write("{canwrite:0x3B3:0x40:0x00:0x00:0x12:0x00:0x00:0x00:0x00}")
        DelayMilliseconds(25)
        Write("{canwrite:0x263:0x02:0x00:0x01:0x00:0x04:0x00:0x00:0x00}")
        DelayMilliseconds(25)
        Write("{canwrite:0x44C:0x00:0x04:0x91:0x00:0x00:0x00:0x00:0x00}")
        Read()
        DelayMilliseconds(1000)
        FlushRXTX()

        #Dimmed Indicators
        Write("{canwrite:0x3B3:0x40:0x00:0x00:0x0C:0x00:0x00:0x00:0x00}")
        DelayMilliseconds(25)
        Write("{canwrite:0x263:0x02:0x00:0x01:0x00:0x04:0x00:0x00:0x00}")
        DelayMilliseconds(25)
        Write("{canwrite:0x44C:0x00:0x04:0x91:0x00:0x00:0x00:0x00:0x00}")
        Read()
        DelayMilliseconds(1000)
        FlushRXTX()

        #All Indicators And Illumination
        Write("{canwrite:0x3B3:0x40:0x00:0xC0:0x12:0x00:0x00:0x00:0x00}")
        DelayMilliseconds(25)
        Write("{canwrite:0x263:0x02:0x00:0x01:0x00:0x04:0x00:0x00:0x00}")
        DelayMilliseconds(25)
        Write("{canwrite:0x44C:0x00:0x04:0x91:0x00:0x00:0x00:0x00:0x00}")
        Read()
        DelayMilliseconds(1000)
        FlushRXTX()

        #All Indicators And Illumination Dimmed
        Write("{canwrite:0x3B3:0x40:0x00:0x60:0x0C:0x00:0x00:0x00:0x00}")
        DelayMilliseconds(25)
        Write("{canwrite:0x263:0x02:0x00:0x01:0x00:0x04:0x00:0x00:0x00}")
        DelayMilliseconds(25)
        Write("{canwrite:0x44C:0x00:0x04:0x91:0x00:0x00:0x00:0x00:0x00}")
        Read()
        DelayMilliseconds(1000)
        FlushRXTX()


        Write("{clearcanmsg}")
        DelayMilliseconds(100)
        Read()
        FlushRXTX()
    }
}