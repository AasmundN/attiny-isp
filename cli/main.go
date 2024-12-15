package main

import (
	"bufio"
	"fmt"
	"os"
	"slices"
	"strings"

	"github.com/alecthomas/kong"
	"go.bug.st/serial"
	"go.bug.st/serial/enumerator"
)

/**
 *
 * Command line tool for ISP programmer
 * Should probably split into multiple packages
 *
 */

/**
 * Application settings
 */
var fileTypes = []string{"bin"}
var usbSerialID = "MCHP3252021800001854"
var serialMode = &serial.Mode{
	BaudRate: 9600,
	Parity:   serial.NoParity,
	DataBits: 8,
	StopBits: serial.OneStopBit,
}

// serial commands
const WRITE = 'w'
const WRITE_ERASE = 'e'
const WRITE_VERIFY = 'v'
const WRITE_VERIFY_ERASE = 'W'
const END_OF_DATA = 0x69 // must find a char that never occurs in programs
const ACK = 'a'
const SUCCESS = 's'

/**
 * Definition of command line interface
 */
type WriteCmd struct {
	Erase  bool `short:"e" help:"Erase memory before writing"`
	Verify bool `short:"v" help:"Verify memory content after writing"`

	Path string `arg:"" name:"path" help:"Path to binary file" type:"path"`
}

var cli struct {
	Write WriteCmd `cmd:"" help:"Write to program memory"`
}

/**
 * Finds and connects to programmer serial port
 */
func serialConnect() (serial.Port, error) {
	ports, err := enumerator.GetDetailedPortsList()

	if err != nil {
		return nil, err
	}

	if len(ports) == 0 {
		return nil, fmt.Errorf("No serial ports found")
	}

	portName := ""
	for _, port := range ports {
		if !port.IsUSB {
			continue
		}

		if port.SerialNumber != usbSerialID {
			continue
		}

		portName = port.Name
	}

	if portName == "" {
		return nil, fmt.Errorf("Could not find programmer")
	}

	port, _ := serial.Open(portName, serialMode)

	return port, nil
}

func readFile(filepath string) ([]byte, error) {
	file, err := os.Open(filepath)

	if err != nil {
		return nil, err
	}
	defer file.Close()

	stats, statsErr := file.Stat()
	if statsErr != nil {
		return nil, statsErr
	}

	var size int64 = stats.Size()
	bytes := make([]byte, size)

	bufr := bufio.NewReader(file)
	_, err = bufr.Read(bytes)

	return bytes, err
}

/**
 * Write command callback
 */
func (w *WriteCmd) Run() error {
	if !slices.Contains(fileTypes, strings.Split(w.Path, ".")[1]) {
		return fmt.Errorf("Invalid file type")
	}

	fmt.Println("Connecting to programmer")

	port, err := serialConnect()

	if err != nil {
		return err
	}

	buff := []byte{WRITE_VERIFY_ERASE}
	port.Write(buff) // set programming mode
	port.Read(buff)  // await acknowledgement

	if buff[0] != byte(ACK) {
		return fmt.Errorf("Could not connect to target")
	}

	fmt.Println("Reading program file")

	bytes, fileErr := readFile(w.Path)

	if fileErr != nil {
		return fileErr
	}

	fmt.Println("Writing program to memory")

	port.Write(bytes) // write program

	buff[0] = byte(END_OF_DATA) // send end of data
	port.Write(buff)
	port.Read(buff) // await acknowledgement

	if buff[0] != byte(SUCCESS) {
		return fmt.Errorf("Programming failed :(")
	}

	fmt.Println("All done :)")

	return nil
}

func main() {
	ctx := kong.Parse(&cli)
	err := ctx.Run()
	ctx.FatalIfErrorf(err)
}
