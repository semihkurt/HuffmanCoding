# Huffman Coding C++ Implementation

A Huffman code is a particular type of optimal prefix code that is commonly used for lossless data compression.
[Huffman Coding](https://en.wikipedia.org/wiki/Huffman_coding)

## Installation

You just need to clone the repository. I have used QMake for building system but it doesn't matter whatever you choose because it plainly uses only CPP libraries.

## Usage

1) Reading input characters from file

If you want to read characters from your File, you just need to change these lines:

```
const std::string FILENAME = "/Users/semih/Development/src/HuffmanCoding/example1.txt";

bool you_wanna_read_from_file = true;
```

One of the important thing is decision of InputEnum
1) EachLetter    -> A, B, A , D , B etc.
2) Bigram		 -> AB, BA, CD, DD etc.

If you want to process each letter change:
```
fillFrequency(tFileRead,InputEnum::EachLetter);
```

2) Giving directly frequency map to the tree

If you want to try just giving sample frequencies, do

```
bool you_wanna_read_from_file = false;
```

and prepare 
```
        frequenciesMap = {
            {"A",45},
            {"B",13},
            {"C",12},
            {"D",16},
            {"E",9},
            {"F",5}
        };
```
like this. I've used [Introduction to Algorithms](https://en.wikipedia.org/wiki/Introduction_to_Algorithms) example in this case.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)