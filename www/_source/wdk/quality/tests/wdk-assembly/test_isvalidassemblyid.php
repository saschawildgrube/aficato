<?php

	require_once(GetWDKDir()."wdk_websitesatellite.inc");
	require_once(GetWDKDir()."wdk_assembly.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test IsValidAssemblyID");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_IsValidAssemblyID(
			$value,
			$bExpectedValue)
		{ 
			$this->Trace("TestCase_IsValidAssemblyID");
		
			$bValue = IsValidAssemblyID($value);
		
			$this->Trace("IsValidAssemblyID(".RenderValue($value).") = ".RenderBool($bValue));
		
			if ($bValue == $bExpectedValue)
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
			$this->TestCase_IsValidAssemblyID("assembly",true);
			$this->TestCase_IsValidAssemblyID("wdk",true);
			$this->TestCase_IsValidAssemblyID("root",true);
			$this->TestCase_IsValidAssemblyID("test",true);
			$this->TestCase_IsValidAssemblyID("abc",true);
			$this->TestCase_IsValidAssemblyID("thisisanassembly",true);
			
			$this->TestCase_IsValidAssemblyID("",false);
			$this->TestCase_IsValidAssemblyID("1",false);
			$this->TestCase_IsValidAssemblyID("ABC",false);
			$this->TestCase_IsValidAssemblyID("Abc",false);
			$this->TestCase_IsValidAssemblyID("abC",false);
			$this->TestCase_IsValidAssemblyID("123yours",false);

		}
		
		
	}
	
	

		
