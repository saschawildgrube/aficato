<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$arrayItems["MODULE_LOGIN"] = $this->GetWebsite()->m_moduleLogin->RenderModule("form");
			$strPostLoginOutput = "";
			if ($this->GetWebsite()->GetAuthUserID() != "")
			{
				$message = new CMessageElement($this->GetWebsite(),"info");
				$strPostLoginOutput = $message->RenderMessage("?TID_INFO_POSTLOGIN?");
			}
			$arrayItems["POSTLOGIN"] = $strPostLoginOutput;
		}
	}
	
