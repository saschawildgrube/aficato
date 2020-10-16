<?php

	class CMessageStackElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"messagestack",
				$strStyleID,
				false,
				true);
		}
		
		function RenderMessageStack($arrayMessages)
		{
			$strMessages = "";
			
			if (is_array($arrayMessages))
			{
				$arrayErrorMessages = ArrayGetValue($arrayMessages,"error");	
				if (is_array($arrayErrorMessages))
				{
					$message = new CMessageElement($this->GetWebsite(),"error");
					foreach ($arrayErrorMessages as $strMessage)
					{
						$strMessages .= $message->RenderMessage($strMessage);
					}	
				}
				
				$arrayWarningMessages = ArrayGetValue($arrayMessages,"warning");	
				if (is_array($arrayWarningMessages))
				{
					$message = new CMessageElement($this->GetWebsite(),"warning");
					foreach ($arrayWarningMessages as $strMessage)
					{
						$strMessages .= $message->RenderMessage($strMessage);
					}	
				}
				
				
				$arrayInfoMessages = ArrayGetValue($arrayMessages,"info");	
				if (is_array($arrayInfoMessages))
				{
					$message = new CMessageElement($this->GetWebsite(),"info");
					foreach ($arrayInfoMessages as $strMessage)
					{
						$strMessages .= $message->RenderMessage($strMessage);
					}	
				}
			}			
			
			$strOutput = $this->GetLayoutHTML(); 
			$this->SetOutputItem(
				"MESSAGES",
				$strMessages);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}


	}

