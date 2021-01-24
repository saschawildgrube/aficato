<?php
	
	require_once(GetWDKDir().'wdk_zip.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK CompressFilesInMemory');
		}
		  

		function TestCase_CompressFilesInMemory(
			$arrayInputFiles,
			$strInputArchiveComment,
			$strExpectedResultHex)
		{ 
			$this->Trace('');
			$this->Trace('');
			$this->Trace('TestCase_CompressFilesInMemory');
			

	
			$this->Trace('arrayInputFiles:');
			$this->Trace($arrayInputFiles);
			$this->Trace('strInputArchiveComment: '.RenderValue($strInputArchiveComment)); 
			$this->Trace('Expected:'); 
			$this->Trace(RenderValue($strExpectedResultHex));

			$strResult = CompressFilesInMemory($arrayInputFiles);
			
			$strResultHex = false;
			if ($strResult != false)
			{
				$strResultHex = RenderHex($strResult);
				if (StringLength($strResultHex) > 22)
				{
					// At some positions there are random values in the result (seems to be a time stamp)
				
					$strResultHex[20] = '?';
					$strResultHex[21] = '?';	
					$strResultHex[22] = '?';	
					$strResultHex[23] = '?';	
					$strResultHex[24] = '?';
					$strResultHex[25] = '?';	
					$strResultHex[26] = '?';	
					$strResultHex[27] = '?';	

					
					$strResultHex[144] = '?';
					$strResultHex[145] = '?';	
					$strResultHex[146] = '?';	
					$strResultHex[147] = '?';	
					$strResultHex[148] = '?';
					$strResultHex[149] = '?';	
					$strResultHex[150] = '?';	
					$strResultHex[151] = '?';	

					
				}
			}

			//$timeNow = time();
			//$this->Trace('Time: '.RenderHex(pack('I',$timeNow)));    
			
			$this->Trace('CompressFilesInMemory returns:');
			$this->Trace(RenderValue($strResultHex));
			$this->Trace('StringLength(): '.StringLength($strResult));
			
			if ($strExpectedResultHex === false)
			{
				if (!($strResult === false))
				{
					$this->Trace('False was expected but not returned.');
					$this->Trace('Testcase FAILED!');	
					$this->SetResult(false);
					return;
				}
			}
			
			if ($strResultHex != $strExpectedResultHex)
			{
				$this->Trace('Hex strings are not identical.');
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

			$arrayInputFiles = array();
			$strInputArchiveComment = '';
			$strExpectedHex = false;
			$this->TestCase_CompressFilesInMemory(
				$arrayInputFiles,
				$strInputArchiveComment,
				$strExpectedHex);

			$arrayInputFiles = array(
				array(
					'contents' => 'This is test contents',
					'archivefilepath' => '/test.txt',
					'archivefilecomment' => 'This is a test file'
					) 
				);
			$strInputArchiveComment = 'This is a test comment';
			$strExpectedHex = '504b0304140000000000????????40df2dd91500000015000000090000002f746573742e74787454686973206973207465737420636f6e74656e7473504b01021403140000000000????????40df2dd91500000015000000090000001300000000000000b681000000002f746573742e74787454686973206973206120746573742066696c65504b050600000000010001004a0000003c0000000000';
			$this->TestCase_CompressFilesInMemory(
				$arrayInputFiles,
				$strInputArchiveComment, 
				$strExpectedHex);
 

		}
	}
	
	

		
