<?php
	
	require_once("unittest_makesql.inc");
	
	class CTest extends CUnitTestMakeSQL
	{
		function __construct()
		{
			parent::__construct("MakeSQL REPLACE");
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
			
			$arraySQL = array(
				"command" => "REPLACE",
				"tables" => "Table1",
				"values" => array(
					array(
						"field" => "id",
						"value" => 1),
					array(
						"field" => "name",
						"value" => "John"),
					array(
						"field" => "age",
						"value" => 42)
					)
				);
			$this->TestCase_MakeSQL(
				$arraySQL,
				"REPLACE INTO /*TablePrefix*/`Table1` SET `id` = 1, `name` = 'John', `age` = 42;");


		
		}
		

	}
	
	

		
