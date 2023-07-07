# query-snippet-generation
Program to generate term-biased snippets for paragraphs of text

We have all used web search tools. In response to a query, a search engine result page (SERP) is
generated, containing (among many other elements) a list of URLs and snippets, with each snippet a
short extract from the corresponding web page. A typical snippet contains around 20–30 words and
is selected from the corresponding document to show the context in which query terms appear. 
For example, the query “web search algorithms” at a commercial web search service resulted (August
2022) in the following URLs (in blue), page titles (in italics), and snippets (with matching key words
in bold) being offered:

![SCR-20230707-flc](https://github.com/ybotf/query-snippet-generation/assets/56858161/5258ab14-31ff-426f-abec-0e9e42d9a23b)

Note how the fourth snippet is actually composed of two shorter segments, indicated by the “. . . ” in
the middle of it. In the other four snippets the generator decided that a single contiguous segment of
text was the best representation.

This program that reads paragraphs of text from stdin, builds a snippet for each paragraph according to certain rules, and then writes those snippets
to stdout. This can be seperated into 3 stages:

## Stage 1
function get word() so that it returns one of three status values: WORD FND if it identified
a word; PARA END if instead of a word, a paragraph boundary was identified; and EOF if the end of the
input file was reached. Paragraphs are identified in the input text by the combination "\n\n", that is,
two consecutive newline characters; or by reaching the end of file after one or more alphabetic words
have been found since the last paragraph boundary A suitable #define appears in the skeleton code.
Once that is done, alter the processing loop in the main() so that it processes one paragraph at
a time rather than one word at a time. To accomplish this step you will probably want to introduce
a new function (perhaps called get paragraph()) that calls get word() repeatedly, to build up a
paragraph of words in an array. You may assume that each paragraph has at most MAX PARA LEN
words in it, each of at most MAX WORD LEN characters.
Output: The required output from this stage is a message for each paragraph that indicates the
paragraph number, and its length in words. See the LMS for examples of what is required.
