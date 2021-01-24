<?php

	class CSigninModule extends CModule
	{
		private $m_arrayErrors;
		private $m_strEmailValidationTokenParameterName;
		private $m_passwordpolicy;
		private $m_strPreferredContent;
		private $m_strContentAfterEmailSent;
		
		function __construct(
			CWebSite $website,
			$strEmailValidationTokenParameterName,
			CPasswordPolicy $passwordpolicy,
			$strPreferredContent = '',
			$strContentAfterEmailSent = ''
			)
		{
			$this->m_arrayErrors = array();
			$this->m_strEmailValidationTokenParameterName = $strEmailValidationTokenParameterName;
			$this->m_passwordpolicy = $passwordpolicy;
			$this->m_strPreferredContent = $strPreferredContent;
			$this->m_strContentAfterEmailSent = $strContentAfterEmailSent;
			parent::__construct($website,'user/signin');
		}
		
		function OnInit()
		{
			$bResult = parent::OnInit();
			if ($this->GetVar($this->m_strEmailValidationTokenParameterName) != '')
			{
				$this->SetState('validation');
			}
			return $bResult;
		}
		
		
		function OnWebsiteInit()
		{
			if ($this->GetState() == 'signinattempt')
			{
				$this->GetWebsite()->UserLogin(
					$this->GetWebsite()->UserNameInputCorrection($this->GetModuleParam('new_username')),
					$this->GetModuleParam('new_password'));
			}
		}		
		
		function OnRenderModule($strState)
		{
			switch ($strState)
			{
				/*
				case 'signinattempt':
				{
					return $this->StateSigninAttempt();	
				}*/
				case 'presignin-username-submit':
				{
					return $this->StatePreSigninUsernameSubmit();
				}
				case 'presignin-useremail-submit':
				{
					return $this->StatePreSigninUserEmailSubmit();
				}				
				case 'presignin-password-submit':
				{
					return $this->StatePreSigninPasswordSubmit();
				}
				case 'presignin-username-suggestions-submit':
				{
					return $this->StatePreSigninUsernameSuggestionsSubmit();
				}
				case 'presignin':
				{
					return $this->StatePreSignin();	
				}
				case 'postsignin':
				{
					return $this->StatePostSignin();	
				}
				default:	
				{
					if ($this->GetVar($this->m_strEmailValidationTokenParameterName) != '')
					{
						return $this->StateValidation();
					}
					return $this->StatePreSignin();
				}
			}
		}

		
		function StatePreSignin()
		{
			$this->SetState('presignin');
			$this->SetModuleVar('new_username','');
			$this->SetModuleVar('new_password','');
			$this->SetModuleVar('new_password2','');
			$this->SetModuleVar('new_useremail','');
			return $this->StatePreSigninUsername();
		}
		
		function StatePreSigninUsername()
		{
			$this->SetState('presignin-username');
			
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'presignin-username-submit';
			if ($this->m_strPreferredContent != '')
			{
				$arrayParams['content'] = $this->m_strPreferredContent;
			}
			$formDef->SetURL($this->GetWebsite()->GetRootURL_PreferHTTPS(),$arrayParams);
			
			$formDef->AddControlText(
				'NEW_USERNAME',
				$this->GetModuleVarName('new_username'),
				'?TID_SIGNIN_LABEL_USERNAME?',
				'',
				15,
				64);

			$formDef->AddControlButtonSubmit('?TID_BUTTON_NEXT?');
			
			$form = new CFormElement($this->GetWebsite(),'default');
			
			$this->SetOutputItem(
				'FORM',
				$form->RenderForm(
					$formDef,
					$this->GetAllModuleVars(),
					$this->m_arrayErrors));
					
			return 'presignin-username';	
		}		
		
		function StatePreSigninUsernameSubmit()
		{
			$this->SetState('presignin-username-submit');
			
			$strNewUserName = $this->GetWebsite()->UserNameInputCorrection($this->GetModuleVar('new_username'));
			if ($strNewUserName == '')
			{
				$this->m_arrayErrors['NEW_USERNAME'] = '?TID_SIGNIN_ERROR_USERNAME_MISSING?';
				return $this->StatePreSigninUsername();
			}
			
			if ($this->GetWebSite()->UserCheckName($strNewUserName) == false)
			{
				$this->SetErrorMessageCode('USERNAME_NOTALLOWED');
				$this->SetModuleVar('new_username','');
				return $this->StatePreSigninUsername();
			}

			$arrayParams = array();
			$arrayParams['user_name'] = $strNewUserName;
			$arrayParams['command'] = 'checkusername';
			$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());
			$consumer->ConsumeWebService('system/user',$arrayParams);
			$strError = $consumer->GetError();
			if ($strError == 'USERNAME_BLACKLISTED')
			{
				$this->SetErrorMessageCode('USERNAME_BLACKLISTED');
				$this->SetModuleVar('new_user_name','');
				return $this->StatePreSigninUsername();
			}
			if ($strError != '')
			{
				return $this->StatePreSigninUsernameSuggestions();
			}
			return $this->StatePreSigninUserEmail();
			
		}
		
		function StatePreSigninUsernameSuggestions()
		{
			$this->SetState('presignin-username-suggestions');
			
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'presignin-username-suggestions-submit';
			if ($this->m_strPreferredContent != '')
			{
				$arrayParams['content'] = $this->m_strPreferredContent;
			}
			$formDef->SetURL($this->GetWebsite()->GetRootURL_PreferHTTPS(),$arrayParams);
			
			$strPreferredUsername = $this->GetWebsite()->UserNameInputCorrection($this->GetModuleVar('new_username'));
			$arrayAlternativeUsernames = array();
			
			$arrayOptions = array();
			$nCount = 0;
			$nIndex = 0;
			while ($nCount < 4)
			{
				$nIndex++;
				$strAlternativeUsername = $strPreferredUsername . $nIndex;

				$arrayParams = array();
				$arrayParams['user_name'] = $strAlternativeUsername;
				$arrayParams['command'] = 'get';
				$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());
				$consumer->ConsumeWebService('system/user',$arrayParams);
				$strError = $consumer->GetError();
				if ($strError != '') 
				{
					if ($nCount == 0)
					{
						$strFirstAlternative = $strAlternativeUsername;
					}
					$arrayOptions[$strAlternativeUsername] = $strAlternativeUsername;
					$nCount++;
				}
			}
			

			$arrayOptions[''] = '?TID_SIGNIN_OPTION_USERINPUT?';
			
			$formDef->AddControlRadio(
				'SUGGESTIONS',
				$this->GetModuleVarName('new_username_alternative'),
				'?TID_SIGNIN_LABEL_ALTERNATIVEUSERNAME?',
				$arrayOptions,
				$strFirstAlternative);
			
			$formDef->AddControlText(
				'NEW_USERNAME',
				$this->GetModuleVarName('new_username'),
				'?TID_SIGNIN_LABEL_ALTERNATIVEUSERNAMEINPUT?',
				$strPreferredUsername,
				15,
				64);

			$formDef->AddControlButtonSubmit('?TID_BUTTON_NEXT?');
			
			$form = new CFormElement($this->GetWebsite(),'default');			
			
			$this->SetOutputItem(
				'FORM',
				$form->RenderForm(
					$formDef,
					array(),
					$this->m_arrayErrors));
					
			$this->SetModuleVar('new_username_alternative','');
					
			return 'presignin-username-alternatives';
		}
		
		function StatePreSigninUsernameSuggestionsSubmit()
		{
			$this->SetState('presignin-username-suggestions-submit');
			
			$strNewUserName = $this->GetModuleVar('new_username_alternative');
			if ($strNewUserName == '')
			{
				$strNewUserName = $this->GetModuleVar('new_username');
			}
			if ($strNewUserName == '')
			{
				$this->m_arrayErrors['NEW_USERNAME'] = '?TID_SIGNIN_ERROR_USERNAME_MISSING?';
				return $this->StatePreSigninUsernameSuggestions();
			}
			$this->SetModuleVar('new_username',$strNewUserName);
			return $this->StatePreSigninUsernameSubmit();
			
			
		}
		
		function StatePreSigninUserEmail()
		{
			$this->SetState('presignin-useremail');
			
			
			$strNewUserName = $this->GetWebsite()->UserNameInputCorrection($this->GetModuleVar('new_username'));
			if ($strNewUserName == '')
			{
				$this->m_arrayErrors['NEW_USERNAME'] = '?TID_SIGNIN_ERROR_USERNAME_MISSING?';
				return $this->StatePreSigninUsername();
			}			
			
			
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'presignin-useremail-submit';
			if ($this->m_strPreferredContent != '')
			{
				$arrayParams['content'] = $this->m_strPreferredContent;
			}
			$formDef->SetURL($this->GetWebsite()->GetRootURL_PreferHTTPS(),$arrayParams);
			
			$formDef->AddControlText(
				'NEW_USEREMAIL',
				$this->GetModuleVarName('new_useremail'),
				'?TID_SIGNIN_LABEL_USEREMAIL?',
				'',
				20,
				255);

			$formDef->AddControlButtonSubmit('?TID_BUTTON_NEXT?');
			
			$form = new CFormElement($this->GetWebsite(),'default');
			
			$this->SetOutputItem(
				'FORM',
				$form->RenderForm(
					$formDef,
					$this->GetAllModuleVars(),
					$this->m_arrayErrors));
					
			return 'presignin-useremail';	
		}	
		
		function StatePreSigninUserEmailSubmit()
		{
			$this->SetState('presignin-useremail-submit');
			
			$strNewUserEmail = $this->GetModuleVar('new_useremail');
			if ($strNewUserEmail == '')
			{
				$this->m_arrayErrors['NEW_USEREMAIL'] = '?TID_SIGNIN_ERROR_USEREMAIL_MISSING?';
				return $this->StatePreSigninUserEmail();
			}
			if (!IsValidEmail($strNewUserEmail))
			{
				$this->m_arrayErrors['NEW_USEREMAIL'] = '?TID_SIGNIN_ERROR_USEREMAIL_INVALID?';
				return $this->StatePreSigninUserEmail();
			}
			
			// all ok, go to the next step
			return $this->StatePreSigninPassword();
			
			
		}
		
		function StatePreSigninPassword()
		{
			$this->SetState('presignin-password');
			
			
			$strNewUserName = $this->GetWebsite()->UserNameInputCorrection($this->GetModuleVar('new_username'));
			if ($strNewUserName == '')
			{
				$this->m_arrayErrors['NEW_USERNAME'] = '?TID_SIGNIN_ERROR_USERNAME_MISSING?';
				return $this->StatePreSigninUsername();
			}
			$strNewUserEmail = $this->GetModuleVar('new_useremail');
			if ($strNewUserEmail == '')
			{
				$this->m_arrayErrors['NEW_USEREMAIL'] = '?TID_SIGNIN_ERROR_USEREMAIL_MISSING?';
				return $this->StatePreSigninUserEmail();
			}
			
			
			
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'presignin-password-submit';
			if ($this->m_strPreferredContent != '')
			{
				$arrayParams['content'] = $this->m_strPreferredContent;
			}
			$formDef->SetURL($this->GetWebsite()->GetRootURL_PreferHTTPS(),$arrayParams);
			
			$formDef->AddControlPassword('NEW_PASSWORD',$this->GetModuleVarName('new_password'),'?TID_SIGNIN_LABEL_PASSWORD?','',15,255);
			$formDef->AddControlPassword('NEW_PASSWORD2',$this->GetModuleVarName('new_password2'),'?TID_SIGNIN_LABEL_PASSWORD_REPEAT?','',15,255);

			if (get_class($this->m_passwordpolicy) == 'CPasswordPolicy')
			{
				$this->IncludeElement('passwordpolicy');				
				$passwordpolicyelement = new CPasswordPolicyElement($this->GetWebsite(),'default');
				$formDef->AddControlStatic(
					'PASSWORDPOLICY',
					$passwordpolicyelement->RenderPasswordPolicyInfo(
						$this->m_passwordpolicy));	
			}

			$formDef->AddControlButtonSubmit('?TID_BUTTON_NEXT?');

			
			$form = new CFormElement($this->GetWebsite(),'default');
			
			$this->SetOutputItem(
				'FORM',
				$form->RenderForm(
					$formDef,
					$this->GetAllModuleVars(),
					$this->m_arrayErrors));
					
			return 'presignin-password';	
		}

		function StatePreSigninPasswordSubmit()
		{
			$this->SetState('presignin-password-submit');
			$strPassword = $this->GetModuleVar('new_password');
			$strPassword2 = $this->GetModuleVar('new_password2');
			
			if ($strPassword == '')
			{
				$this->SetModuleVar('new_password','');
				$this->SetModuleVar('new_password2','');
				$this->m_arrayErrors['NEW_PASSWORD'] = '?TID_SIGNIN_ERROR_PASSWORD_MISSING?';
				return $this->StatePreSigninPassword();
			}


			if (is_object($this->m_passwordpolicy))
			{
				$arrayErrors = array();
				if (!$this->m_passwordpolicy->CheckPassword($strPassword,$arrayErrors))
				{
					$this->SetModuleVar('new_password','');
					$this->SetModuleVar('new_password2','');
					
					$this->IncludeElement('passwordpolicy');	
					$passwordpolicyelement = new CPasswordPolicyElement($this->GetWebsite(),'default');
									
					//$this->m_arrayErrors['NEW_PASSWORD'] = '?TID_SIGNIN_ERROR_PASSWORD_POLICY?';
					$this->m_arrayErrors['NEW_PASSWORD'] = $passwordpolicyelement->RenderPasswordPolicyErrors($arrayErrors);
					return $this->StatePreSigninPassword();					
				}	
			}
			
			if ($strPassword != $strPassword2)
			{
				$this->SetModuleVar('new_password','');
				$this->SetModuleVar('new_password2','');
				$this->m_arrayErrors['NEW_PASSWORD2'] = '?TID_SIGNIN_ERROR_PASSWORD_MISMATCH?';
				return $this->StatePreSigninPassword();
			}
			
			
			// all ok, go to the next step
			return $this->StateSigninAttempt();
			
			
		}
		
		function StateSigninAttempt()
		{
			$this->SetState('signinattempt');
	
			$strUserName = $this->GetWebsite()->UserNameInputCorrection($this->GetModuleVar('new_username'));
			$strUserEmail = $this->GetModuleVar('new_useremail');
			$strPassword = $this->GetModuleVar('new_password');
			$strPassword2 = $this->GetModuleVar('new_password2');
			
			$this->SetModuleVar('new_password','');
			$this->SetModuleVar('new_password2','');
			$this->SetModuleVar('new_username','');
			$this->SetModuleVar('new_useremail','');			
			
			// Last check before asking the system/user webservice			
			if ($strUserName == '')
			{
				return $this->StatePreSigninUsernameSubmit();
			}
			if ($strPassword == '')
			{
				return $this->StatePreSigninPasswordSubmit();
			}
			if ($strPassword != $strPassword2)
			{
				return $this->StatePreSigninPasswordSubmit();
			}
			if (!IsValidEmail($strUserEmail))
			{
				return $this->StatePreSigninUserEmailSubmit();	
			}
			
			$nValidationDurationSeconds = 60*60*24*3;
			$timeValidationEnd = GetTimeNow() + $nValidationDurationSeconds;

			
			$strError = '';
			$arrayResults = array();
			$strUserID = $this->GetWebsite()->UserSignin(
				$strUserName,
				$strPassword,
				array(
					'user_email' => $strUserEmail,
					'user_email_validation' => true,
					'user_email_validation_duration' => $nValidationDurationSeconds
					),
					$strError,
					$arrayResults);
			
			if ($strError != '')
			{
				$this->SetErrorMessageCode($strError);
				return $this->StatePreSignin();
			}

			if ($strUserID == false)
			{
				$this->SetErrorMessageCode('UNKNOWN');
				return $this->StatePreSignin();
			}

			
			$strUserID = ArrayGetValue($arrayResults,'NEW_USER_ID');
			$strToken = ArrayGetValue($arrayResults,'VALIDATION_TOKEN');

			$arrayParams = array();
			$arrayParams[$this->m_strEmailValidationTokenParameterName] = $strToken;
			$strValidationLink = MakeURL($this->GetWebsite()->GetRootURL_PreferHTTPS(),$arrayParams,false);
				
		
			$arrayTags = array();
			$arrayTags['VALIDATION_LINK'] = $strValidationLink;
			$arrayTags['SITENAME'] = $this->GetWebsite()->GetSiteName();
			$arrayTags['SITETEAM'] = $this->GetWebsite()->GetSiteTeamName();
			$arrayTags['USERNAME'] = $strUserName;
			$arrayTags['USEREMAIL'] = $strUserEmail;
			$arrayTags['VALIDATION_END_DATETIME'] = $this->GetWebsite()->RenderDateTime($timeValidationEnd);
			$arrayTags['VALIDATION_END_TIMEZONE'] = $this->GetWebsite()->GetTimeZone();
			$strMessage = $this->GetWebsite()->LoadString('TID_SIGNIN_EMAIL_BODY');
			$strMessage = ReplaceTagsWDK($strMessage,$arrayTags);

			$strSubject = $this->GetWebsite()->LoadString('TID_SIGNIN_EMAIL_SUBJECT');
			$strSubject = ReplaceTagsWDK($strSubject,$arrayTags);				
				
			SendMail(
				$strUserEmail,
				$strSubject,
				$strMessage,
				$this->GetWebsite()->GetSiteTeamEmail(),
				$this->GetWebsite()->GetSiteTeamName(),
				array());

			$message = new CMessageElement($this->GetWebsite(),'info');
			$strMessage = $message->RenderMessage('?TID_SIGNIN_INFO_EMAIL_SENT?');
			$this->SetOutputItem('MESSAGE',$strMessage);
			
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = 'modify';
			if ($this->m_strContentAfterEmailSent != '')
			{
				$arrayParams['content'] = $this->m_strContentAfterEmailSent;
			}
			$strBackLinkURL = $this->GetWebsite()->MakeURL($arrayParams);
			$link = new CLinkElement($this->GetWebsite(),'default');
			$strBackLink = $link->RenderLink($strBackLinkURL,'?TID_SIGNIN_LINK_BACK?');
			$this->SetOutputItem('BACKLINK',$strBackLink);

			return 'signin-attempt-mailsent';
		}
		

		
		function StateValidation()
		{ 
			$this->SetState('validation');
			$this->SetState('');
			
			$strToken = $this->GetVar($this->m_strEmailValidationTokenParameterName);
			$this->SetVar($this->m_strEmailValidationTokenParameterName,'');
			if ($strToken == '')
			{
				return 'presignin';	
			}
			
			$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			$arrayParams = array();
			$arrayParams['user_email_validation_token'] = $strToken;
			$arrayParams['command'] = 'validateemail';
			$consumer->ConsumeWebService('system/user',$arrayParams);
			$strError = $consumer->GetError();
			$this->Trace($strError);
			if ($strError == 'TOKEN_NOT_FOUND')
			{
				$this->GetWebsite()->AddWarningMessageText('?TID_SIGNIN_ERROR_TOKEN_NOT_FOUND?');
				return $this->StatePreSignin();
			}
			if ($strError != '')
			{
				$this->SetErrorMessageCode($strError);
				return $this->StatePreSignin();
			}
			
			$strUsername = $consumer->GetResultValue('USER','USER_NAME');
			$this->Trace('strUsername = '.$strUsername);
			
			$this->GetWebsite()->SetAuthUsernameVar($strUsername);
				
			$strUsername = $this->GetWebsite()->GetAuthUsernameVar();
				
			$this->Trace('GetAuthUsernameVar: '.$strUsername);
			
			$this->SystemEvent(
				'INFORMATIONAL',
				'USER_SIGNIN',
				$strUsername,
				array(
					'username' => $strUsername,
					'useremail' => $consumer->GetResultValue('USER','USER_EMAIL')));
			
			return $this->StateSigninSuccess();
		}
	
		function StateSigninSuccess()
		{
			$this->SetState('signinsuccess');
					
			$strMessage = '?TID_SIGNIN_INFO_WELCOME?';
			$message = new CMessageElement($this->GetWebsite(),'info');
			$this->SetOutputItem('MESSAGE',
				$message->RenderMessage(
					$strMessage));
			
			return 'postsignin';	
		}		
		
		function StatePostSignin()
		{
			$this->SetState('postsignin');
			if ($this->GetWebsite()->GetAuthUserID() == '')
			{
				return $this->StatePreSignin();	
			}
			$this->SetOutputItem('MESSAGE', '');
			return 'postsignin';	
		}

	}

