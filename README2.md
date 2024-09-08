- [G-Code](#g-code)
  - [Comments](#comments)
  - [Response](#response)
  - [Line Number](#line-number)
  - [Checksum \& CRC](#checksum--crc)
  - [Buffering](#buffering)
  - [Errors](#errors)
  - [G-Commands](#g-commands)
    - [`G0` \& `G1`: Move](#g0--g1-move)
    - [`G28`: Move to Origin (Home)](#g28-move-to-origin-home)
    - [`G90`: Set Absolute Positioning](#g90-set-absolute-positioning)
    - [`G91`: Set Relative Positioning](#g91-set-relative-positioning)
    - [`G92`: Set Position](#g92-set-position)
    - [`G92.1`: Reset Axis Offset (Native Machine Coordinates)](#g921-reset-axis-offset-native-machine-coordinates)
  - [M-Commands](#m-commands)
    - [`M110`: Set Current Line Number](#m110-set-current-line-number)
    - [`M112`: Emergency Stop](#m112-emergency-stop)



# G-Code

## Comments

All characters are ignored following a semicolon `;`. Typically, the serial
sender will strip these comments to save bandwidth.

## Response

Once a command is parsed, but not necessarily executed, one of the following
responses are sent.

- `ok Nn Bn`
- `error:n Nn`

## Line Number

Line numbers can optionally be included in a command using the `N` parameter.
If a line number is included, the response will also include this line number.

Line numbers are expected to be sequential. If a non-sequential line number is
received, then an error is generated. The line number can be manually set using
the [`M110`](#m110-set-current-line-number) command.

## Checksum & CRC

A checksum or CRC can optionally be included using the `*` parameter. The
checksum/CRC is calculated using all bytes up to, but not including, the `*`
character. The checksum/CRC must always be the last parameter (before comments).

The checksum is an 8 bit value calculated by XOR-ing the bytes using ASCII
decimal encoding. It will include 1, 2, or 3 digits following the `*`.

The CRC uses CCITT CRC16 and the polynomial 0x1021. It will always include 5
decimal digits.

## Buffering

TODO

## Errors

| Error Code | Description |
|------------|-------------|
| 1 | Syntax error
| 2 | Line number error
| 3 | Checksum or CRC error


## G-Commands


### `G0` & `G1`: Move

**Usage**
> `G1 Xn Yn Zn Fn`

**Parameters**
- `X` Position to move to on the X axis
- `Y` Position to move to on the Y axis
- `Z` Position to move to on the Z axis
- `F` Feedrate or speed in mm/min

The `G0` (Rapid Move) and `G1` (Linear Move) are treated the same.


### `G28`: Move to Origin (Home)

**Usage**
> `G28 X Y Z Rn`

**Parameters**
- `X` Flag to go to X axis origin
- `Y` Flag to go to Y axis origin
- `Z` Flag to go to Z axis origin
- `R` Raise be mm before homing

Home to endstops


### `G90`: Set Absolute Positioning

TODO



### `G91`: Set Relative Positioning

TODO



### `G92`: Set Position

TODO



### `G92.1`: Reset Axis Offset (Native Machine Coordinates)

TODO



## M-Commands


### `M110`: Set Current Line Number

Resets the current line number for error detection.

**Usage**
> `M110 N123`

**Parameters**
> `Nn` Line number


### `M112`: Emergency Stop

TODO

