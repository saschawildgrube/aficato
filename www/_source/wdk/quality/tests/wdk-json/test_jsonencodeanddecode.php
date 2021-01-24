<?php
	
	require_once(GetWDKDir().'wdk_json.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK JsonEncode and JsonDecode');
		}
		  

		function TestCase_JsonEncode_JsonDecode(
			$arrayInput,
			$strExpectedResult, 
			$arrayExpectedResult)
		{ 
			$this->Trace('');
			$this->Trace('');
			$this->Trace('TestCase_JsonEncode_JsonDecode');
	
			$this->Trace('arrayInput:');
			$this->Trace($arrayInput);
			$this->Trace('Expected:'); 
			$this->Trace(RenderValue($strExpectedResult));

			$strResult = JsonEncode($arrayInput);

			$this->Trace('JsonEncode result:');
			$this->Trace(RenderValue($strResult));
			$this->Trace('StringLength(): '.StringLength($strResult));
			
			if ($strExpectedResult === false)
			{
				if (!($strResult === false))
				{
					$this->Trace('Testcase FAILED!');	
					$this->SetResult(false);
					return;
				}
			}
	
			if (StringRemoveControlChars($strResult) != StringRemoveControlChars($strExpectedResult))
			{
				$this->Trace('Testcase FAILED!');	
				$this->SetResult(false);
				return;
			}
			
			$arrayResult = JsonDecode($strResult);
			$this->Trace('JsonDecode result:');
			$this->Trace($arrayResult);
			if ($arrayResult != $arrayExpectedResult)
			{
				$this->Trace('Testcase FAILED!');	
				$this->SetResult(false);
				return;
			}
			

			$this->Trace('Testcase PASSED!');
		}


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);

			$arrayInput = array();
			$strJsonExpected = '[]';
			$arrayExpected = array();
			$this->TestCase_JsonEncode_JsonDecode(
				$arrayInput,
				$strJsonExpected,
				$arrayExpected);
			
			$arrayInput = '';
			$strJsonExpected = false;
			$arrayExpected = false;
			$this->TestCase_JsonEncode_JsonDecode(
				$arrayInput,
				$strJsonExpected,
				$arrayExpected);			
			
			$arrayInput = array(
				1 => 'a',
				2 => 'b',
				3 => 'c'
				);
			$strJsonExpected =
'{
    "1": "a",
    "2": "b",
    "3": "c"
}';
			$arrayExpected = $arrayInput;
			$this->TestCase_JsonEncode_JsonDecode(
				$arrayInput,
				$strJsonExpected,
				$arrayExpected);
				
			$arrayInput = array(
				array('Keys','Values'),
				array(1,'a'),
				array(2,'b'),
				array(3,'c')
				);
			$strJsonExpected =
'[
    [
        "Keys",
        "Values"
    ],
    [
        1,
        "a"
    ],
    [
        2,
        "b"
    ],
    [
        3,
        "c"
    ]
]';
			$arrayExpected = $arrayInput;
			$this->TestCase_JsonEncode_JsonDecode(
				$arrayInput,
				$strJsonExpected,
				$arrayExpected);

			$arrayInput = array(
				10 => 'a',
				20 => 'b',
				30 => 'c'
				);
			$strJsonExpected =
'{
    "10": "a",
    "20": "b",
    "30": "c"
}';
			$arrayExpected = $arrayInput;
			$this->TestCase_JsonEncode_JsonDecode(
				$arrayInput,
				$strJsonExpected,
				$arrayExpected);
				
			$arrayInput = array(
				'Colors' => array('Red','Green','Blue'),
				'Numbers' => array(3.14,9.9),
				'Booleans' => array(true,false)
				);
			$strJsonExpected =
'{
    "Colors": [
        "Red",
        "Green",
        "Blue"
    ],
    "Numbers": [
        3.14,
        9.9
    ],
    "Booleans": [
        true,
        false
    ]
}';
			$arrayExpected = $arrayInput;
			$this->TestCase_JsonEncode_JsonDecode(
				$arrayInput,
				$strJsonExpected,
				$arrayExpected);



		}
	}
	
	

		
