<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Profiling different hash methods");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_HashMethodProfiling($strMethod)
		{
			$nIterations = 3;
			
			$this->Trace("Testcase_HashMethodProfiling: $strMethod");
			$this->Trace("Using $strMethod $nIterations times.");
			$stopwatch = new CStopWatch();
			$stopwatch->Start();
			for ($nCount = 0; $nCount < $nIterations; $nCount++)
			{
				$strHash = MakeHash("$nCount","",$strMethod);	
			}
			$stopwatch->Stop();
			$fElapsedSeconds = $stopwatch->GetSeconds();
			$fHashesPerSecond = $nIterations / $fElapsedSeconds;
			$this->Trace("Elapsed seconds  : ".RenderNumber($fElapsedSeconds,5)." seconds");
			$this->Trace("Hashes per second: ".RenderNumber($fHashesPerSecond,5));
			$this->Trace("");
			
		}

		function OnTest()
		{
			parent::OnTest();
			
			$this->TestCase_HashMethodProfiling("sha1");
			$this->TestCase_HashMethodProfiling("bcrypt7");
			$this->TestCase_HashMethodProfiling("bcrypt12");

		}
		

	}
	
	

		
