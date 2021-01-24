<?php
	
	require_once("testcase_encryption-asymmetric.inc");
		
	class CTest extends CEncryptAsymmetricUnitTest
	{
		function __construct()
		{
			parent::__construct();
		}
		
		function OnInit()
		{
			$this->SetActive(false); // key generation on headless systems takes too long on some systems.
			return parent::OnInit();
		}



		function OnTest()
		{
			parent::OnTest();

			$this->SetResult(true);
			
			$strMessage = "";
			$strMessage .= "COURSE CATEGORY AND NAME: kajhdalskjdhalskjdahlsdjhalsdkjhasldkjahsldkjahsldkajhsdlkahd18726312873618\n";
			$strMessage .= "START DATETIME          : 769786978698769876\n";
			$strMessage .= "CATEGORYID              : uzdaoiusdzoasiudzoasiudzaosiud\n";
			$strMessage .= "COURSEID                : jhalkjdhaslkjdhalskdjhalskjdh\n";
			$strMessage .= "\n";
			$strMessage .= "NAME   : צהצüצהצüצהüצהüצה\n";
			$strMessage .= "ADDRESS: צהצüצהצüצהüצהüצה\n";
			$strMessage .= "PHONE  : 123456789\n";
			$strMessage .= "EMAIL  : sascha@wildgrube.com\n";
			$strMessage .= "MEMBER : true\n";
			$strMessage .= "\n";
			for ($nIndex = 0; $nIndex < 3; $nIndex++)
			{
				$strMessage .= 
					$nIndex
					." ".
					"audults"
					." | PARTICIPANTS: ".
					"3"
					." | PRICE: ".  
					"30"
					." ". 
					"EUR\n";
			}
			$strMessage .= "\n";
			$strMessage .= "DEBT ORDER         : true\n";
			$strMessage .= "BANK ACCOUNT OWNER : test test\n";
			$strMessage .= "BANK ACCOUNT NUMBER: 50021020202\n";
			$strMessage .= "BANK ID            : 50010060\n";
			$strMessage .= "BANK NAME          : ACME Bהnk";

			$this->TestCase_EncryptAsymmetric(
				$strMessage);

			$this->TestCase_EncryptAsymmetric(
				StringEncodeUTF8($strMessage));


/*			
			$strPayload = utf8_encode($strPayloadRaw);
			
			$this->Trace("IsStringUTF8(\"$strPayloadRaw\") returns ".RenderBool(IsStringUTF8($strPayloadRaw))."");
			if ($strPayload == $strPayloadRaw)
			{
				$this->Trace("utf8_encode had no effect on payload.");
			}
	*/		
			
			


			
		}
		
	
		
		
	}
	
	

		
