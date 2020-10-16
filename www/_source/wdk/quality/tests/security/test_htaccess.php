<?php
	
	require_once(GetWDKDir().'wdk_unittest.inc');
		
	class CTest extends CUnitTest 
	{
		function __construct()
		{
			parent::__construct('.htaccess');
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
		
			$this->TestCase_CheckURL(
				'http://'.GetRootURL().'_source/wdk/quality/testfiles/htaccess.txt',
				array(),
				array('THIS FILE SHOULD NOT BE ACCESSIBLE VIA HTTP')
				);
		}
	}
		
