<?php
	
	require_once(GetWDKDir()."wdk_mysql.inc");
	
	class CUnitTestMakeSQL extends CUnitTest
	{
		function TestCase_MakeSQL($arraySQL,$strExpectedResult)
		{
			$this->Trace("TestCase_MakeSQL");
			$this->Trace("arraySQL:");
			$this->Trace($arraySQL);
			$this->Trace("Expected result:");
			$this->Trace(RenderValue($strExpectedResult));
			$strError = "";
			$strResult = MakeSQL($arraySQL,$strError);
			if ($strResult === false)
			{
				$this->Trace("MakeSQL() returned false");
				$this->Trace("MakeSQL() error: \"$strError\"");
			}
			else
			{
				$this->Trace("MakeSQL() returns: ");
				$this->Trace(RenderValue($strResult));
			}
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

	}
		
