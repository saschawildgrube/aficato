<?php
	
	require_once("testcase_encryption-asymmetric.inc");
		
	class CTest extends CEncryptAsymmetricUnitTest
	{
		function __construct()
		{
			parent::__construct();
		}
		
		function CallbackInit()
		{
			$this->SetActive(false); // key generation on headless systems takes too long on some systems.
			return parent::CallbackInit();
		}



		function CallbackTest()
		{
			parent::CallbackTest();

			$this->SetResult(true);
			
			
			$this->TestCase_EncryptAsymmetric(
				"Dies ist ein Test"); 
			
			$this->TestCase_EncryptAsymmetric(
				"A secret message.\nWith some more lines of text\nHurray\nUmlauts: ÄÖÜ"); 

			$this->TestCase_EncryptAsymmetric(
				u("A secret message.\nWith some more lines of text\nUTF-8\nUmlauts: ÄÖÜ")); 


			$this->TestCase_EncryptAsymmetric(
				"A secret message.\nWith some more lines of text\nHurray\nUmlauts: ÄÖÜ\nWith blanks ");


		}
		
	
		
		
	}
	
	

		
