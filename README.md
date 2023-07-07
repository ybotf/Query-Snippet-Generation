# Query Snippet Generation
Program to generate term-biased snippets for paragraphs of text

**'test3-out.txt' is an example of the output with the query term 'alice children poison drink bottle' for text 'data3.txt'**


We have all used web search tools. In response to a query, a search engine result page (SERP) is generated, containing (among many other elements) a list of URLs and snippets, with each snippet a short extract from the corresponding web page. A typical snippet contains around 20–30 words and is selected from the corresponding document to show the context in which query terms appear.  For example, the query “web search algorithms” at a commercial web search service resulted (August 2022) in the following URLs (in blue), page titles (in italics), and snippets (with matching key words in bold) being offered:

![SCR-20230707-flc](https://github.com/ybotf/query-snippet-generation/assets/56858161/5258ab14-31ff-426f-abec-0e9e42d9a23b)

Note how the fourth snippet is actually composed of two shorter segments, indicated by the “. . . ” in the middle of it. In the other four snippets the generator decided that a single contiguous segment of text was the best representation.

This program that reads paragraphs of text from stdin, builds a snippet for each paragraph according to certain rules, and then writes those snippets to stdout. This can be seperated into 3 stages:

## Stage 1 - Reading Text
A function get_word() returns one three status values: WORD_FND if it identifes a word; PARA_END if instead of a word, a paragraph boundary was identified; and EOF if the end of the input file was reached. Paragraphs are identified in the input text by the combination "\n\n", that is, two consecutive newline characters; or by reaching the end of file after one or more alphabetic words have been found since the last paragraph boundary. The output from this stage is a message for each paragraph that indicates the paragraph number, and its length in words.

## Stage 2 - Matching Words and Printing Text
A function that writes each paragraph’s words across the page, making sure that no line contains more than 72 characters. Each “word” that is output should be separated from the previous word in the same line by a single blank character. Any words in each paragraph that match any query terms in a case-insensitive manner  are “bolded” in the output text by putting “*" before and after them. For example, if “web” is supplied as one of the query terms, then the word “Web;” in the input text must place “*Web*;” into the output text. 

## Stage 3 - Building Snippet
The score of a snippet relative to a set of query terms is calculated as:
- Add 15/(s + 10) points, where s is the start word of the snippet, counting from the first word of the paragraph as word number zero; plus
-  Add l/2 points for each different term that appears in the snippet, where l is the length of that query term; plus
-  Add 1.0 points for every other repetition of any query term that appears multiple times in the snippet; plus
-  Add 0.6 points if the snippet starts with the word immediately following a punctuated word (taking word “−1” to also be punctuated); plus
-  Add 0.3 points if the snippet ends with a punctuated word; and then
-  Subtract 0.1 points for each word that the snippet exceeds MIN SNIPPET LEN in length.
   
Snippets cannot be longer than MAX_SNIPPET_LEN words. Similarly, snippets cannot be shorter than MIN SNIPPET LEN words, except if the input paragraph is also shorter than MIN_SNIPPET_LEN words, in which case the whole input paragraph always becomes the snippet. If a snippet ends with a punctuated word that has a period “.” or a question mark “?” or a exclamation mark “!” attached, then no
“. . . ” should be added. All other cases should have dots added immediately after the last word in the
snippet, even if it takes the last output line past 72 characters.

The highest-scoring contiguous snippet is identified for each input paragraph. In the case of scores that are equal, select the snippet that starts closest to the beginning of the paragraph. And if there are still ties, print out the shortest legal maximum-score snippet starting at that point

(altered descrpition written by Alistair Moffat)
