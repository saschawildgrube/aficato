<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringWordWrap");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			//$this->SetActive(false);
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_StringWordWrap($strString,$nWidth,$strEOL,$bCut,$strExpectedResult)
		{
			$this->Trace("TestCase_StringWordWrap");
			$this->Trace("nWidth: $nWidth");
			$this->Trace("bCut: ".RenderBool($bCut));
			$this->Trace("strString:");
			$this->Trace("$strString");
			$this->Trace("Expected Result:");
			$this->Trace("$strExpectedResult");
			$strResult = StringWordWrap($strString,$nWidth,$strEOL,$bCut);
			$this->Trace("StringWordWrap returns:");
			$this->Trace("$strResult");
			if ($strResult == $strExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);
			}
			$this->Trace("");
			
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();

			$this->TestCase_StringWordWrap(
				"The quick brown fox jumps over the lazy dog.",
				10,
				"\n",
				false,
				"The quick\nbrown fox\njumps over\nthe lazy\ndog.");

			$this->TestCase_StringWordWrap(
				"The quick brown fox jumps over the lazy dog.\nA long time ago in a Galaxy far far away...",
				10,
				"\n",
				false,
				"The quick\nbrown fox\njumps over\nthe lazy\ndog.\nA long\ntime ago\nin a\nGalaxy far\nfar\naway...");

			$this->TestCase_StringWordWrap(
				"Loram ipsom maladasidesudawiduwuma malaue",
				20,
				"\n",
				true,
				"Loram ipsom\nmaladasidesudawiduwu\nma malaue");

			
		
			$this->TestCase_StringWordWrap(
				"12345678901234567890",
				3,
				"\n",
				true,
				"123\n456\n789\n012\n345\n678\n90");

			$this->TestCase_StringWordWrap(
				"12345678901234567890",
				3,
				"\n",
				false,
				"12345678901234567890");


			$this->TestCase_StringWordWrap(
				u("äöüäöüäöüäöüäöüäöü"),
				3,
				"\n",
				true,
				u("äöü\näöü\näöü\näöü\näöü\näöü"));

			$this->TestCase_StringWordWrap(
				u("äöüäöüäöüäöüäöüäöü"),
				3,
				"\n",
				false,
				u("äöüäöüäöüäöüäöüäöü"));

			$this->TestCase_StringWordWrap(
				u("123 123 1 2312 31 23 123"),
				3,
				"\n",
				true,
				u("123\n123\n1\n231\n2\n31\n23\n123"));

			$this->TestCase_StringWordWrap(
				u("123 123 1 2312 31 23 123"),
				3,
				"\n",
				false,
				u("123\n123\n1\n2312\n31\n23\n123"));


			$this->TestCase_StringWordWrap(
				u("äöü äöü ä öüäö üä öü äöü"),
				3,
				"\n",
				true,
				u("äöü\näöü\nä\nöüä\nö\nüä\nöü\näöü"));

			$this->TestCase_StringWordWrap(
				u("äöü äöü ä öüäö üä öü äöü"),
				3,
				"\n",
				false,
				u("äöü\näöü\nä\nöüäö\nüä\nöü\näöü"));

			$this->TestCase_StringWordWrap(
				u("\n\n\n"),
				3,
				"\n",
				false,
				u("\n\n\n"));


			$this->TestCase_StringWordWrap(
				u(""),
				3,
				"\n",
				false,
				u(""));

			$this->TestCase_StringWordWrap(
				u(" \n "),
				3,
				"\n",
				false,
				u("\n"));
	
		}
		

	}
	
	

		
