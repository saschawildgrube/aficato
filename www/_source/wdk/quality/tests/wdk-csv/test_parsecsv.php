<?php

	require_once(GetWDKDir()."wdk_csv.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("CSV");
		}
		

		function TestCase_ParseCSV(
			$arrayExpectedResult,
			$strRawData,
			$bHeaderRow = true,
			$IncludeHeadersInResult = true,
			$strSeparator = ',',
			$arrayCommentTokens = array(),
			$strQuote = '"',
			$strEscapedQuote = '""',
			$bNewLineInQuotedValue = false)
		{ 
			$this->Trace("TestCase_CSV");
	
			$this->Trace("strRawData:\n>>>\n$strRawData\n<<<");
			$this->Trace("Expected Result:");
			$this->Trace($arrayExpectedResult);
	
	
			$arrayData = ParseCSV(
				$strRawData,
				$bHeaderRow,
				$IncludeHeadersInResult,
				$strSeparator,
				$arrayCommentTokens,
				$strQuote,
				$strEscapedQuote,
				$bNewLineInQuotedValue);
			
			$this->Trace("Result:");
			$this->Trace($arrayData);
	
			if ($arrayData == $arrayExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);	
			}
			$this->Trace("");
			$this->Trace("");
		}


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);
			
		
			$strRawData =
'COLUMN1;COLUMN2;COLUMN3
"some quoted text with a semi-colon;";23;line one//a comment
more text;0.23;line two';

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "some quoted text with a semi-colon;",
					"COLUMN2" => "23",
					"COLUMN3" => "line one"),
				array(
					"COLUMN1" => "more text",
					"COLUMN2" => "0.23",
					"COLUMN3" => "line two")
				);
		
			
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('//'),
				'"',
				'""',
				true);


			$strRawData =
'COLUMN1;COLUMN2;COLUMN3
"some quoted text with a semi-colon;";23 //a comment with a separator ; and some text after it"
more text;0.23;line two';

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "some quoted text with a semi-colon;",
					"COLUMN2" => "23",
					"COLUMN3" => ""),
				array(
					"COLUMN1" => "more text",
					"COLUMN2" => "0.23",
					"COLUMN3" => "line two")
				);
		
			
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('//'),
				'"',
				'""',
				true);



			$strRawData =
'COLUMN1;COLUMN2;COLUMN3
"some quoted text";"23"//a comment with a separator ; and some text after it
more text;0.23;line two';

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "some quoted text",
					"COLUMN2" => "23",
					"COLUMN3" => ""),
				array(
					"COLUMN1" => "more text",
					"COLUMN2" => "0.23",
					"COLUMN3" => "line two")
				);
		
			
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('//'),
				'"',
				'""',
				true);


			$strRawData =
'COLUMN1;COLUMN2;COLUMN3
"some quoted text";"23" //a comment with a separator ; and some text after it and a whitespace before the comment
more text;0.23;line two';

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "some quoted text",
					"COLUMN2" => "23",
					"COLUMN3" => ""),
				array(
					"COLUMN1" => "more text",
					"COLUMN2" => "0.23",
					"COLUMN3" => "line two")
				);
		
			
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('//'),
				'"',
				'""',
				true);




			$strRawData =
'COLUMN1;COLUMN2;COLUMN3
"some quoted text with a semi-colon;";23;line one
more text;0.23;line two';

			$arrayExpectedResult = array(
				array(
					"COLUMN1" => "some quoted text with a semi-colon;",
					"COLUMN2" => "23",
					"COLUMN3" => "line one"),
				array(
					"COLUMN1" => "more text",
					"COLUMN2" => "0.23",
					"COLUMN3" => "line two")
				);
		
			
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				false,
				';',
				array('//'),
				'"',
				'""',
				true);




			$strRawData =
'COLUMN1;COLUMN2;COLUMN3
"some quoted text with a semi-colon;";23;line one//a comment
"more text with inline ""quotes""";0.23		;line two';

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "some quoted text with a semi-colon;",
					"COLUMN2" => "23",
					"COLUMN3" => "line one"),
				array(
					"COLUMN1" => 'more text with inline "quotes"',
					"COLUMN2" => "0.23",
					"COLUMN3" => "line two")
				);
		
			
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('//'),
				'"',
				'""',
				true);


$strRawData =
"COLUMN1;COLUMN2;COLUMN3
\"some quoted text with a semi-colon; and a new\nline\";23;line one//a comment
\"more text with inline comment // and \"\"quotes\"\"\";0.23;line two";

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "some quoted text with a semi-colon; and a new\nline",
					"COLUMN2" => "23",
					"COLUMN3" => "line one"),
				array(
					"COLUMN1" => 'more text with inline comment // and "quotes"',
					"COLUMN2" => "0.23",
					"COLUMN3" => "line two")
				);
		
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('//'),
				'"',
				'""',
				true);



$strRawData =
"COLUMN1;COLUMN2;COLUMN3
text with a quote at the end\";23;line one
more text;0.23;line two with a quote at the end\"";

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "text with a quote at the end\"",
					"COLUMN2" => "23",
					"COLUMN3" => "line one"),
				array(
					"COLUMN1" => 'more text',
					"COLUMN2" => "0.23",
					"COLUMN3" => "line two with a quote at the end\"")
				);
		
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('//'),
				'"',
				'""',
				false);


$strRawData =
"COLUMN1;COLUMN2;COLUMN3
\"some quoted text with a semi-colon; and a new\nline\";23;line one//a comment
\"more text with inline comment // and \"\"quotes\"\"\";0.23;line two";

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "some quoted text with a semi-colon; and a new",
					"COLUMN2" => "",
					"COLUMN3" => ""),
				array(
					"COLUMN1" => "line\"",
					"COLUMN2" => "23",
					"COLUMN3" => "line one"),
				array(
					"COLUMN1" => 'more text with inline comment // and "quotes"',
					"COLUMN2" => "0.23",
					"COLUMN3" => "line two")
				);
		
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('//'),
				'"',
				'""',
				false);



$strRawData =
"COLUMN1;COLUMN2;COLUMN3\n".
"\"some quoted text with a semi-colon; and a new\nline\";23;line one\n".
"#\"more text with inline \"\"quotes\"\"\";0.23;line two\n".
"\"even more quoted text with a semi-colon;\";24;line three (since line two is commented)";

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "some quoted text with a semi-colon; and a new\nline",
					"COLUMN2" => "23",
					"COLUMN3" => "line one"),
				array(
					"COLUMN1" => "even more quoted text with a semi-colon;",
					"COLUMN2" => "24",
					"COLUMN3" => "line three (since line two is commented)"), 
				);
		
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('#'),
				'"',
				'""',
				true);

$strRawData =
"COLUMN1;COLUMN2;COLUMN3\n".
"\"some quoted text with a semi-colon; and a new\nline\";23;line one\n".
"\n".
"\n".
"\"even more quoted text with a semi-colon;\";24;line two (since line two lines are empty)";

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "some quoted text with a semi-colon; and a new\nline",
					"COLUMN2" => "23",
					"COLUMN3" => "line one"),
				array(
					"COLUMN1" => "even more quoted text with a semi-colon;",
					"COLUMN2" => "24",
					"COLUMN3" => "line two (since line two lines are empty)"), 
				);
		
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('#'),
				'"',
				'""',
				true);


$strRawData =
"COLUMN1;COLUMN2;COLUMN3\n".
"1.1;1.2;\n".
"2.1;2.2;2.3\n".
"3.1;3.2;3.3\n".
"4.1;;4.3\n".
";;\n".
";6.2;6.3\n";

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "1.1",
					"COLUMN2" => "1.2",
					"COLUMN3" => ""),
				array(
					"COLUMN1" => "2.1",
					"COLUMN2" => "2.2",
					"COLUMN3" => "2.3"), 
				array(
					"COLUMN1" => "3.1",
					"COLUMN2" => "3.2",
					"COLUMN3" => "3.3"), 
				array(
					"COLUMN1" => "4.1",
					"COLUMN2" => "",
					"COLUMN3" => "4.3"), 
				array(
					"COLUMN1" => "",
					"COLUMN2" => "",
					"COLUMN3" => ""), 
				array(
					"COLUMN1" => "",
					"COLUMN2" => "6.2",
					"COLUMN3" => "6.3"), 
				);
		
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('#'),
				'"',
				'""',
				true);



$strRawData =
"COLUMN1;COLUMN2;COLUMN3\n".
"1.1;1.2;\n".
"2.1;2.2;2.3\n".
"3.1;3.2;3.3\n".
"4.1;;4.3\n".
"\n";

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "1.1",
					"COLUMN2" => "1.2",
					"COLUMN3" => ""),
				array(
					"COLUMN1" => "2.1",
					"COLUMN2" => "2.2",
					"COLUMN3" => "2.3"), 
				array(
					"COLUMN1" => "3.1",
					"COLUMN2" => "3.2",
					"COLUMN3" => "3.3"), 
				array(
					"COLUMN1" => "4.1",
					"COLUMN2" => "",
					"COLUMN3" => "4.3")
				);
		
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('#'),
				'"',
				'""',
				true);




$strRawData =
"COLUMN1;COLUMN2;COLUMN3\n".
"1.1;1.2;\n".
"2.1;2.2;2.3\n".
"\n".
"3.1;3.2;3.3\n".
"4.1;;4.3\n".
"\n";

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "1.1",
					"COLUMN2" => "1.2",
					"COLUMN3" => ""),
				array(
					"COLUMN1" => "2.1",
					"COLUMN2" => "2.2",
					"COLUMN3" => "2.3"), 
				array(
					"COLUMN1" => "3.1",
					"COLUMN2" => "3.2",
					"COLUMN3" => "3.3"), 
				array(
					"COLUMN1" => "4.1",
					"COLUMN2" => "",
					"COLUMN3" => "4.3")
				);
		
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('#'),
				'"',
				'""',
				true);




			$strRawData =
'COLUMN1;COLUMN2;COLUMN3
#"some quoted text with a semi-colon;";23;line one
"more text with inline ""quotes""";0.23;line two';

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "more text with inline \"quotes\"",
					"COLUMN2" => "0.23",
					"COLUMN3" => "line two")
				);
		
			
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('#'),
				'"',
				'""',
				true);


			$strRawData =
'COLUMN1;COLUMN2;COLUMN3
#"some quoted text with a semi-colon;";23;line one AND whitespaces at the end of the file
"more text with inline ""quotes""";0.23;line two                  ';

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "more text with inline \"quotes\"",
					"COLUMN2" => "0.23",
					"COLUMN3" => "line two")
				);
		
			
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('#'),
				'"',
				'""',
				true);
				


			$strRawData =
'COLUMN1;COLUMN2;COLUMN3


';

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3")
				);
		
			
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('#'),
				'"',
				'""',
				true);

			$strRawData =
'COLUMN1;COLUMN2;COLUMN3
#No data';

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3")
				);
		
			
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('#'),
				'"',
				'""',
				true);



			$strRawData =
'COLUMN1;COLUMN2;COLUMN3 //white space before the comment and a comment
a;b;c';

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "a",
					"COLUMN2" => "b",
					"COLUMN3" => "c")					
				);
		
			
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('//'),
				'"',
				'""',
				true);



			$strRawData = 
"A;B;C
A1;B1;C1
A2;B2;C2";

			$arrayExpectedResult = array(
				array("A","B","C"),
				array("A"=>"A1","B"=>"B1","C"=>"C1"),
				array("A"=>"A2","B"=>"B2","C"=>"C2")
				);
		
			
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('//'),
				'"',
				'""',
				true);


			$strRawData =
'"A","B","C"
"","B1","C1"
"A2","","C2"
"A3","B3",""';

			$arrayExpectedResult = array(
				array("A","B","C"),
				array("A"=>"","B"=>"B1","C"=>"C1"),
				array("A"=>"A2","B"=>"","C"=>"C2"),
				array("A"=>"A3","B"=>"B3","C"=>"")
				);
		
			
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				',',
				array('//'),
				'"',
				'""',
				true);




			$strRawData = u(
'"A","U","O"
"äÄä","üÜü®","öÖö"
äAä,üUü,"öOö"
AAAä,UUUü,OOOö');

			$arrayExpectedResult = array(
				array("A","U","O"),
				array("A"=>u("äÄä")	,"U"=>u("üÜü®")	,"O"=>u("öÖö")),
				array("A"=>u("äAä")	,"U"=>u("üUü")	,"O"=>u("öOö")),
				array("A"=>u("AAAä")	,"U"=>u("UUUü"),"O"=>u("OOOö"))
				);
		
			 
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				',',
				array('//'),
				'"',
				'""',
				true);



			$strRawData = u(
'"A","U","O"
"äÄä"		, "üÜü®" ,"öÖö"
äAä,    üUü,    "öOö"
AAAä, "UUUü" ,OOOö');

			$arrayExpectedResult = array(
				array("A","U","O"),
				array("A"=>u("äÄä"),	"U"=>u("üÜü®"),"O"=>u("öÖö")),
				array("A"=>u("äAä"),	"U"=>u("üUü"),	"O"=>u("öOö")),
				array("A"=>u("AAAä"),"U"=>u("UUUü"),"O"=>u("OOOö"))
				);
		
			 
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				',',
				array('//'),
				'"',
				'""',
				true);


			$strRawData = u(
'"A","U","O"
"äÄä"		, "üÜü®" ,"öÖö"     
äAä,    üUü,    "öOö"  aaa
AAAä, "UUUü" ,OOOö//test');

			$arrayExpectedResult = array(
				array("A","U","O"),
				array(u("äÄä"),	u("üÜü®"),u("öÖö")),
				array(u("äAä"),	u("üUü"),	u("öOö")),
				array(u("AAAä"),u("UUUü"),u("OOOö"))
				);
		
			 
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				false,
				false,
				',',
				array('//'),
				'"',
				'""',
				true);



			$strRawData = u(
'"A""","""U""","O"
"äÄä"		, "üÜü®" ,"öÖö"
äAä,    üUü,    "öOö"
AAAä, "UUU ""ü""" ,OOOö');

			$arrayExpectedResult = array(
				array("A\"","\"U\"","O"),
				array("A\""=>u("äÄä"),	"\"U\""=>u("üÜü®"),			"O"=>u("öÖö")),
				array("A\""=>u("äAä"),	"\"U\""=>u("üUü"),			"O"=>u("öOö")),
				array("A\""=>u("AAAä"),	"\"U\""=>u("UUU \"ü\""),	"O"=>u("OOOö"))
				);
		
			 
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				',',
				array('//'),
				'"',
				'""',
				true);



$strRawData =
"COLUMN1;COLUMN2;COLUMN3\n".
"\"some quoted text with a semi-colon; and a new\nline\";23;line one\n".
"\n".
"\n".
"\"even more quoted text with a semi-colon;\";24;line two (since line two lines are empty)";

			$arrayExpectedResult = array(
				array(
					"COLUMN1",
					"COLUMN2",
					"COLUMN3"),
				array(
					"COLUMN1" => "some quoted text with a semi-colon; and a new",
					"COLUMN2" => "",
					"COLUMN3" => ""),
				array(
					"COLUMN1" => "line\"",
					"COLUMN2" => "23",
					"COLUMN3" => "line one"),
				array(
					"COLUMN1" => "even more quoted text with a semi-colon;",
					"COLUMN2" => "24",
					"COLUMN3" => "line two (since line two lines are empty)"), 
				);
		
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array(),
				'"',
				null,
				false);



$strRawData =
"COLUMN1;COLUMN2;COLUMN3\n".
"\"some quoted text with a semi-colon; and a new\nline\";23;line one\n".
"\n".
"\n".
"\"even more quoted text with a semi-colon;\";24;line two (since line two lines are empty)";

			$arrayExpectedResult = array(
				array(
					"COLUMN1" => "some quoted text with a semi-colon; and a new",
					"COLUMN2" => "",
					"COLUMN3" => ""),
				array(
					"COLUMN1" => "line\"",
					"COLUMN2" => "23",
					"COLUMN3" => "line one"),
				array(
					"COLUMN1" => "even more quoted text with a semi-colon;",
					"COLUMN2" => "24",
					"COLUMN3" => "line two (since line two lines are empty)"), 
				);
		
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				false,
				';',
				array(),
				'"',
				null,
				false);




			$strRawData = "";
			$arrayExpectedResult = array();
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				true,
				';',
				array('//'),
				'"',
				'""',
				true);

				

				
		}
	
		
		
	}
	
	

		
