<?php
	
	require_once(GetWDKDir().'wdk_unittest.inc');
		
	class CTest extends CUnitTest 
	{
		function __construct()
		{
			parent::__construct('.htaccess');
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function OnTest()
		{
			parent::OnTest();
		
			$this->TestCase_CheckURL(
				'http://'.GetRootURL().'_source/wdk/quality/testfiles/htaccess.txt',
				array(),
				array('THIS FILE SHOULD NOT BE ACCESSIBLE VIA HTTP')
				);
		}
	}
		
