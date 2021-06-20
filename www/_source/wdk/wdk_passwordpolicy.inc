<?php


	class CPasswordPolicy extends CDataContainer
	{
		function __construct()
		{
			parent::__construct();
			$this->Init();
		}

		function Init()
		{
			$this->SetMinimumLength(1);
			$this->SetRequiredCharsAlpha(false);
			$this->SetRequiredCharsAlphaLowercase(false);
			$this->SetRequiredCharsAlphaUppercase(false);
			$this->SetRequiredCharsNumeric(false);
		}

		function CheckPassword($strPassword, &$arrayErrors = array())
		{
			if (!$this->IsValid())
			{
				$arrayErrors[] = "PASSWORDPOLICY_INVALID";
				return false;	
			}
			$bResult = true;
			$nMinimumLength = $this->GetMinimumLength();
			if (StringLength($strPassword) < $nMinimumLength)
			{
				$arrayErrors[] = "PASSWORDPOLICY_MINIMUM_LENGTH";
				$bResult = false;  
			}
			
			
			$bRequiredCharsAlphaUppercase = $this->GetRequiredCharsAlphaUppercase();
			if ($bRequiredCharsAlphaUppercase == true)
			{
				$bOK = false;
				$arrayChars = str_split($strPassword);
				foreach ($arrayChars as $char)
				{
					$charUpper = StringUpperCase($char);
					if ((ctype_alpha($char) == true) && ($charUpper == $char))
					{
						$bOK = true;
					}	
				}
				if (!$bOK)
				{
					$arrayErrors[] = "PASSWORDPOLICY_REQUIRED_CHARS_ALPHA_UPPERCASE";
					$bResult = false; 
				}
			}
			
			$bRequiredCharsAlphaLowercase = $this->GetRequiredCharsAlphaLowercase();
			if ($bRequiredCharsAlphaLowercase == true)
			{
				$bOK = false;
				$arrayChars = str_split($strPassword);
				foreach ($arrayChars as $char)
				{
					$charUpper = StringLowerCase($char);
					if ((ctype_alpha($char) == true) && ($charUpper == $char))
					{
						$bOK = true;
					}	
				}
				if (!$bOK)
				{
					$arrayErrors[] = "PASSWORDPOLICY_REQUIRED_CHARS_ALPHA_LOWERCASE";
					$bResult = false; 
				}
			}
			
			
			if ((!$bRequiredCharsAlphaUppercase) && (!$bRequiredCharsAlphaLowercase))
			{
				$bRequiredCharsAlpha = $this->GetRequiredCharsAlpha();
				if ($bRequiredCharsAlpha == true)
				{
					$bOK = false;
					$arrayChars = str_split($strPassword);
					foreach ($arrayChars as $char)
					{
						if (ctype_alpha($char) == true)
						{
							$bOK = true;
						}	
					}
					if (!$bOK)
					{
						$arrayErrors[] = "PASSWORDPOLICY_REQUIRED_CHARS_ALPHA";
						$bResult = false; 
					}
				}
			}
			
			
			$bRequiredCharsNumeric = $this->GetRequiredCharsNumeric();
			if ($bRequiredCharsNumeric == true)
			{
				$bOK = false;
				$arrayChars = str_split($strPassword);
				foreach ($arrayChars as $char)
				{
					if (ctype_digit($char) == true)
					{
						$bOK = true;
					}	
				}
				if (!$bOK)
				{
					$arrayErrors[] = "PASSWORDPOLICY_REQUIRED_CHARS_NUMERIC";
					$bResult = false; 
				}
			}

			return $bResult;	
		}
		
		function GeneratePassword()
		{
			// This function is not yet implemented!
			return "1";
		}
		
		function SetPolicy($arrayData)
		{
			$this->Init();
			$this->MergeDataArray($arrayData);
		}
		 
		
		function SetMinimumLength($nMinimumLength)
		{
			$this->SetValue($nMinimumLength,"minimumlength");
		}
		function GetMinimumLength()
		{
			return $this->GetValue("minimumlength");
		}
		
		function SetRequiredCharsAlpha($bValue = true)
		{
			$this->SetValue(($bValue)?(true):(false),"requiredcharsalpha");
		}
		function GetRequiredCharsAlpha()
		{
			return $this->GetValue("requiredcharsalpha");
		}
		
		function SetRequiredCharsAlphaUppercase($bValue = true)
		{
			$this->SetValue(($bValue)?(true):(false),"requiredcharsalphauppercase");
		}
		function GetRequiredCharsAlphaUppercase()
		{
			return $this->GetValue("requiredcharsalphauppercase");
		}		

		function SetRequiredCharsAlphaLowercase($bValue = true)
		{
			$this->SetValue(($bValue)?(true):(false),"requiredcharsalphalowercase");
		}
		function GetRequiredCharsAlphaLowercase()
		{
			return $this->GetValue("requiredcharsalphalowercase");
		}


		function SetRequiredCharsNumeric($bValue = true)
		{
			$this->SetValue(($bValue)?(true):(false),"requiredcharsnumeric");
		}
		function GetRequiredCharsNumeric()
		{
			return $this->GetValue("requiredcharsnumeric");
		}
			
		
		function IsValid()
		{
			$nMinimumLength = $this->GetMinimumLength();
			if ($nMinimumLength < 1)
			{
				return false;	
			}
			if (!is_bool($this->GetRequiredCharsAlpha()))
			{
				return false;	
			}
			if (!is_bool($this->GetRequiredCharsAlphaUppercase()))
			{
				return false;	
			}
			if (!is_bool($this->GetRequiredCharsAlphaLowercase()))
			{
				return false;	
			}
			if (!is_bool($this->GetRequiredCharsNumeric()))
			{
				return false;	
			}
			return true; 
		}
		
		
		
		
	}



