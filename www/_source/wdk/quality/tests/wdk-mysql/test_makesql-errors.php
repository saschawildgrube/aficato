<?php
	
	require_once("unittest_makesql.inc");
	
	class CTest extends CUnitTestMakeSQL
	{
		function __construct()
		{
			parent::__construct("MakeSQL Error cases");
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

			$arraySQL = false;
			$this->TestCase_MakeSQL(
				$arraySQL,
				false);


			$arraySQL = "";
			$this->TestCase_MakeSQL(
				$arraySQL,
				false);


			$arraySQL = 23;
			$this->TestCase_MakeSQL(
				$arraySQL,
				false);


			$arraySQL = array();
			$this->TestCase_MakeSQL(
				$arraySQL,
				false);

			$arraySQL = array(
				"command" => "SELECT",
				"tables" => "Table1",
				"bogus" => "hello world");
			$this->TestCase_MakeSQL(
				$arraySQL,
				false);

		
		}
		

	}
	
	

		
