<?php
	
	class CTest extends CUnitTest
	{
		private $m_bExpressionA;
		private $m_bExpressionB;
		
		function __construct()
		{
			parent::__construct("Check PHP behaviour: Operator logical or");
			$this->m_bExpressionA = false;
			$this->m_bExpressionB = false;
		}
		
		function FunctionA()
		{
			$this->m_bExpressionA = true;
			return false;	
		}

		function FunctionB()
		{
			$this->m_bExpressionB = false;
			return true;	
		}


		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->SetResult(true);
			
			$this->Trace("Given the logical expression: c = a || b;");
			$this->Trace("If expression a is false, expression b will not be executed.");
					
			$bResult = $this->FunctionA() || $this->FunctionB();
			
			if ($bResult != true)
			{
				$this->Trace("FAILED: result is not true!");
				$this->SetResult(false);
			}

			if ($this->m_bExpressionA != true)
			{
				$this->Trace("FAILED: Expression a was not processed!");
				$this->SetResult(false);
			}

			if ($this->m_bExpressionB == true)
			{
				$this->Trace("FAILED: Expression b was processed!"); 
				$this->SetResult(false);
			}
		}
		
			
	}
	
	

		
