<?php
	
	require_once("unittest_makesql.inc");
	
	class CTest extends CUnitTestMakeSQL
	{
		function __construct()
		{
			parent::__construct("MakeSQL INSERT");
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
			
			$arraySQL = array(
				"command" => "INSERT",
				"tables" => "Table1",
				"values" => array(
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
				"INSERT INTO /*TablePrefix*/`Table1` SET `name` = 'John', `age` = 42;");


			$arraySQL = array(
				"command" => "INSERT",
				"tables" => "Table1",
				"values" => array(
					array(
						"field" => "name",
						"value" => "John"),
					array(
						"field" => "age",
						"value" => 42)
					),
				"on_duplicate_key_update" => true
				);
			$this->TestCase_MakeSQL(
				$arraySQL,
				"INSERT INTO /*TablePrefix*/`Table1` SET `name` = 'John', `age` = 42 ON DUPLICATE KEY UPDATE `name` = 'John', `age` = 42;");
	
	
			$arraySQL = array(
				"command" => "INSERT",
				"tables" => "Table1",
				"values" => array(
					array(
						"field" => "name",
						"value" => "John"),
					array(
						"field" => "age",
						"value" => 42)
					),
				"on_duplicate_key_update" => array(
					array(
						"field" => "name",
						"value" => "Smith")
					)
				);
			$this->TestCase_MakeSQL(
				$arraySQL,
				"INSERT INTO /*TablePrefix*/`Table1` SET `name` = 'John', `age` = 42 ON DUPLICATE KEY UPDATE `name` = 'Smith';");
		
		
		
		}
		

	}
	
	

		
