<?php
	
	require_once(GetWDKDir()."wdk_encryption-symmetric.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK MakeInitVector");
		}
		

		function OnInit()
		{
			$this->SetResult(true);
			if (!IsMcryptSupported())
			{
				$this->Trace("To install mcrypt support:");
				$this->Trace("sudo apt-get install php5-mcrypt");
				$this->Trace("sudo /etc/init.d/apache2 restart");
				$this->Trace("");
				$this->SetActive(false);
			}
			return parent::OnInit();	
		}
		
		
		function TestCase_MakeInitVector()
		{
			
			

			$this->Trace("TestCase_EncryptDecrypt END");
			$this->Trace("");
				
		}

		function OnTest()
		{
			parent::OnTest();
			
			$stopwatch = new CStopWatch();
			
			$stopwatch->Start();
			$initvector = MakeInitVector();
			$stopwatch->Stop();
			$this->Trace("MakeInitVector() speed (seconds): ".RenderNumber($stopwatch->GetSeconds(),3));
			$strInitVectorHex = RenderHex($initvector);
			$this->Trace("RenderHex(initvector) = \"$strInitVectorHex\"");
			$nLength = StringLength($strInitVectorHex);
			$this->Trace("StringLength(RenderHex(initvector) = \"$nLength\"");

			if ($nLength != 64)
			{
				$this->Trace("StringLength(RenderHex(initvector) is not 64");
				$this->SetResult(false);	
			}
			
			$this->ProfilingTrace();
			
		}
		
		
	}
	
	

		
