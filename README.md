# Run-Length Encoding (RLE)
COMP 211 Checkoff 3, Henry Gottfried

## What I explored
Run-length encoding is one of the simplest lossless compression techniques. Instead of storing every symbol, it stores each run as a count and a symbol:

    WWWWWBBBWWWW  ->  5W3B4W

I started with the basic idea and then looked at when RLE actually saves space, how the encoder works step by step, how real formats improve on it, and where RLE shows up inside bigger compressors. I also wrote an encoder and decoder in C.

## Most interesting discoveries

### 1. RLE can make data bigger.
Each run becomes a pair (count + symbol), so data with no repeats doubles in size: `ABCD` becomes `1A1B1C1D`. RLE only saves space when the average run length is more than 2.

### 2. The format has to be unambiguous, and the encoder and decoder have to agree on it.
If counts can be any number of digits, `12` is ambiguous: is it a count of 12 or the data "1" and "2"? My first encoder wrote counts of any length but my decoder read one digit at a time, so `12A` decoded wrong and read past the end of the string. The fix was to cap runs at 9 so every count is exactly one digit. Longer runs just get split (`9A3A`), and digits in the input work fine (`12` -> `1112` -> `12`).

### 3. The encoder only needs one pass and almost no memory.
It reads the input once and only remembers the current symbol and its count, so it's O(n) time and O(1) extra memory. It can even process data as it arrives. A pair gets written in three situations: the next symbol is different, the count hits the max, or the input ends. Forgetting the last run is the most common RLE bug.

### 4. Real formats fix the doubling problem.
- PackBits (TIFF) uses packets that either say "repeat this byte k times" or "copy the next k bytes as-is," so `ABCDEFGH` is 9 bytes instead of 16.
- Fax machines use binary RLE: only black and white, so they store only the counts (`5 3 4`). Rows always start with white, so a row that starts with black begins with a white run of length 0.

### 5. RLE is usually one step in a bigger system.
Most real data doesn't have long runs, so compressors first transform the data to create runs, then use RLE. For example, in JPEG, rounding turns most of the detail values into long runs of zeros.

RLE itself is always lossless. When JPEG loses quality, it's from the rounding step, not from RLE.

### Takeaway
Use RLE by itself when the data already has long runs (simple images, icons, fax scans). For everything else, it works best as a fast step inside a bigger compressor that reshapes the data first.

## Running the code

    cd code
    gcc -Wall -o rle rle.c
    ./rle

The program encodes the four demo examples, decodes them back to the originals, and tries one bad input to show the decoder rejects it instead of crashing.

Output:

    Encoding:
    WWWWWBBBWWWW -> 5W3B4W
    ABCD -> 1A1B1C1D
    AAAAAAAAAAAA -> 9A3A
    12 -> 1112

    Decoding:
    5W3B4W -> WWWWWBBBWWWW
    1A1B1C1D -> ABCD
    9A3A -> AAAAAAAAAAAA
    1112 -> 12
    A5 -> invalid input

Limitation: this version works on text strings. Since it stops at `'\0'`, it can't handle binary data like images. To do that, you'd use a byte for the count instead of a digit (storing count − 1 so one byte covers runs of 1–256) and read with `fread` until EOF instead of looping over a string.

## Presentation
See `slides/`.

## Resources
- [AI conversation](https://claude.ai/share/efa4c2c6-6a16-4d36-94b9-eb1ef0f9e6f6) (Claude)
- [Run-length encoding video](https://www.youtube.com/watch?v=h-TGpvQYyDI) (YouTube)
- [What is Run-Length Encoding? (RLE)](https://www.youtube.com/watch?v=p-K3TzFQvJo), aloneguid (YouTube)
- [Run-Length Encoding](https://www.youtube.com/watch?v=cAAeSn8_aCU), MizComputing (YouTube)
