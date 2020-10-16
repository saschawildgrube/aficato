<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Element Wiki");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit(); 
			//$this->SetActive(false);
			return true;
		}
		
		function TestCase_WikiElement(
			$strURL,
			$strExpected)
		{ 
			$this->Trace("");
			$this->Trace("TestCase_WikiElement");
			$this->TestCase_CheckURL($strURL,array($strExpected));
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			$this->SetResult(true);


			$strExpected = "<div id=\"title\"></div>
<h1>Title</h1>
<div id=\"subtitles\"></div>
<h2>Subtitles</h2>
First paragraph<br/>
Second Paragraph
<div id=\"lists\"></div>
<h1>Lists</h1>
<ul>
<li>Bullet Level 1<ul>
<li>Bullet Level 2</li>
</ul>
</li>
</ul>
<ol start=\"1\" type=\"1\">
<li>Numeric Level 1<ol start=\"1\" type=\"1\">
<li>Numeric Level 2</li>
</ol>
</li>
</ol>

<div id=\"formatting\"></div>
<h1>Formatting</h1>
normal <span style=\"font-style: italic\">italic</span> <span style=\"font-weight: bold\">bold</span> <span style=\"font-weight: bold\"><span style=\"font-style: italic\">italic and bold</span></span>
<div style=\"
	padding:1em;
	border:1px dashed #2f6fab;
	color:black;
	background-color:#f9f9f9;
	line-height:1.1em;
	font-family:monospace;
	\">
Preformatted&nbsp;Text
</div> 
<hr/>
<div id=\"links\"></div>
<h1>Links</h1>
<ul>
<li><a href=\"http://".GetRootURL()."quality/testwebsite/en/index/#\">index</a></li>
<li><a href=\"http://".GetRootURL()."quality/testwebsite/en/index/#\">Go to index page</a></li>
<li><a href=\"http://www.example.com\" target=\"_blank\">http://www.example.com</a></li>
<li><a href=\"http://www.example.com\" target=\"_blank\">Example</a></li>
<li><a href=\"http://".GetRootURL()."quality/testwebsite/download/test.pdf\" target=\"_blank\">Test PDF</a></li>
<li><a href=\"http://www.example.com/(with_bracets)\" target=\"_blank\">Example with Bracets</a></li>
</ul>

<div id=\"embeddedhtml\"></div>
<h1>Embedded HTML</h1>
This is some text with html <span style=\"color:red\">formatting</span>.
<div id=\"definitionlists\"></div>
<h1>Definition Lists</h1>
<dl>
<dt style=\"font-weight: bold;\">Alpha</dt>
<dd>This first letter in the Greek alphabet</dd>
<dt style=\"font-weight: bold;\">Zulu</dt>
<dd>Represents the letter Z in the NATO alphabet</dd>
<dd>An African tribe</dd>
</dl>"
;
			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-element-wiki1";
			$this->TestCase_WikiElement( 
				$strURL,
				$strExpected);


			$strExpected = '
<div id="2x2"></div>
<h1>2x2</h1>
<table><tbody>
<tr>
<th>
Header 1</th>
<th>
Header 2</th>
</tr>
<tr>
<td>
Data 11</td>
<td>
Data 12</td>
</tr>
<tr>
<td>
Data 21</td>
<td>
Data 22</td>
</tr>
</tbody></table>

<div id="3x3"></div>
<h1>3x3</h1>
<table><tbody>
<tr>
<th>
Header 1</th>
<th>
Header 2</th>
<th>
Header 3</th>
</tr>
<tr>
<td>
Data 11</td>
<td>
Data 12</td>
<td>
Data 13</td>
</tr>
<tr>
<td>
Data 21</td>
<td>
Data 22</td>
<td>
Data 23</td>
</tr>
<tr>
<td>
Data 31</td>
<td>
Data 32</td>
<td>
Data 33</td>
</tr>
</tbody></table>

<div id="nested"></div>
<h1>Nested</h1>
<table><tbody>
<tr>
<th>
Header 1</th>
<th>
Header 2</th>
</tr>
<tr>
<td>
<table><tbody>
<tr>
<th>
H1</th>
<th>
H2</th>
</tr>
<tr>
<td>
D1</td>
<td>
D2</td>
</tr>
</tbody></table>
</td>
<td>
&nbsp;</td>
</tr>
</tbody></table>

<div id="pre"></div>
<h1>Pre</h1>
<table><tbody>
<tr>
<th>
Header 1</th>
<th>
Header 2</th>
</tr>
<tr>
<td>
<div style="
	padding:1em;
	border:1px dashed #2f6fab;
	color:black;
	background-color:#f9f9f9;
	line-height:1.1em;
	font-family:monospace;
	">
{|<br/>!H1<br/>!H2<br/>|-<br/>|D1<br/>|D2<br/>|}
</div> 
</td>
<td>
<span style="font-weight: bold">Formatted</span></td>
</tr>
</tbody></table>

<div id="empty"></div>
<h1>Empty</h1>
<table><tbody>
</tbody></table>

<div id="mixedheaders"></div>
<h1>Mixed Headers</h1>
<table><tbody>
<tr>
<th>
Header</th>
<td>
Data</td>
<th>
Header</th>
</tr>
<tr>
<td>
Data</td>
<th>
Header</th>
<td>
Data</td>
</tr>
<tr>
<th>
Header</th>
<td>
Data</td>
<th>
Header</th>
</tr>
</tbody></table>

<div id="singlelinerows"></div>
<h1>Single Line Rows</h1>
<table><tbody>
<tr>
<th>
Header1</th>
<th>
Header2</th>
<th>
Header3</th>
</tr>
<tr>
<td>
Data1</td>
<td>
Data2</td>
<td>
Data3</td>
</tr>
</tbody></table>
';			



			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-element-wiki2";
			$this->TestCase_WikiElement(
				$strURL,
				$strExpected);
			
			
			$strExpected = "
<div id=\"sourcecode\"></div>
<h1>Source Code</h1>
Here is some source code:
<pre class=\"sourcecode\">
	function <a href=\"http://".GetRootURL()."quality/testwebsite/en/helloworld/#\">HelloWorld</a>(\$strName)
	{
		print(\"Hello \$strName\");
	}
</pre>";							
				
			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-element-wiki3";
			$this->TestCase_WikiElement(
				$strURL,
				$strExpected);				
			
			

			$strExpected = '<div id="title"></div>
<h1>Title</h1>
<ul>
<li>Bullet item</li>
</ul>
<ol start="1" type="1">
<li>Numbered item</li>
</ol>
Some text
<ol start="1" type="1">
<li>Numbered item</li>
</ol>
<ul>
<li>Bullet item</li>
</ul>
'
;
			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-element-wiki4";
			$this->TestCase_WikiElement( 
				$strURL,
				$strExpected);
			
			
			

		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			return true;
		}
		 
		
	} 
	
	
		


		
