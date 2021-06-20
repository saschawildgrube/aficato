<?php
	
	require_once(GetWDKDir().'wdk_wiki.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('ParseWiki');
		}
		

		function TestCase_ParseWiki(
			$strWiki,
			$arrayExpectedResult)
		{ 
			$this->Trace('TestCase_ParseWiki');
	 
			$this->Trace('Input:');
			$this->Trace($strWiki);

			$this->Trace('Expected:');
			$this->Trace($arrayExpectedResult);


			$arrayResult = ParseWiki($strWiki);
			$this->Trace('ParseWiki returns:');
			$this->Trace($arrayResult);
			
			if ($arrayResult != $arrayExpectedResult)
			{
				$this->Trace('Expected result does not match.');	
				$this->Trace('Testcase FAILED!');	
				$this->SetResult(false);	
				$this->Trace('');
				$this->Trace('');
				return;
			}
			
			$this->Trace('Testcase PASSED!');
			$this->Trace('');
			$this->Trace('');
		}


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);

			$strWiki = '';
			$arrayExpectedResult = array(
				'lines' => array(
					array(
						'number' => 0,
						'raw' => ''
						)
					)
				);
			$this->TestCase_ParseWiki($strWiki,$arrayExpectedResult);



			$strWiki =
'=Header 1=
Text line 1
Text line 2
=Header 2=
Text line 3
Text line 4';
			$arrayExpectedResult = array(
				'lines' => array(
					array(
						'number' => 0,
						'raw' => '=Header 1='
						),
					array(
						'number' => 1,
						'raw' => 'Text line 1'
						),
					array(
						'number' => 2,
						'raw' => 'Text line 2'
						),
					array(
						'number' => 3,
						'raw' => '=Header 2='
						),
					array(
						'number' => 4,
						'raw' => 'Text line 3'
						),
					array(
						'number' => 5,
						'raw' => 'Text line 4'
						)
					)
				);
			$this->TestCase_ParseWiki($strWiki,$arrayExpectedResult);



		}
		
		
	}
	
	


		
