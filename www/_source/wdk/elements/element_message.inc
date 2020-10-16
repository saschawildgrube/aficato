<?php

	class CMessageElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"message",
				$strStyleID,
				false,
				true);
		}
		
		function RenderMessage($strMessageText)
		{
			$strOutput = $this->GetLayoutHTML(); 
			$this->SetOutputItem(
				"MESSAGE_TEXT",
				$strMessageText);

			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}


	}

