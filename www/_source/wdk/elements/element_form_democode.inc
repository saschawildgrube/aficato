$this->IncludeElement('text');
$text = new CTextElement($this->GetWebsite(),'default');

$arrayErrors = array();
$formDef = new CFormDefinition();

$formDef->SetURL(
	$this->GetWebsite()->GetRootURL(),
	$this->GetWebsite()->GetSessionAwareArrayVars());
	
$formDef->SetHiddenValue(
	$this->GetModuleVarName('somehiddencounter'),
	intval($this->GetModuleVar('somehiddencounter')) + 1);
	
$formDef->AddControlReadOnly(
	'READONLY1',
	'Hidden counter',
	intval($this->GetModuleVar('somehiddencounter')));

$formDef->AddControlText(
	'TEXT1',
	$this->GetModuleVarName('text1'),
	'Text',
	$this->GetModuleVar('text1'),
	40,
	40);

$formDef->AddControlStatic(
	'STATIC1',
	'Some static text');


$formDef->AddControlText(
	'TEXT2',
	$this->GetModuleVarName('text2'),
	'Text (Displays an error if empty)',
	$this->GetModuleVar('text2'),
	40,
	40);
	
if ($this->GetModuleVar('text2') == '')
{
	$arrayErrors['TEXT2'] = 'This field must not be empty!';	
}


$formDef->AddControlTextArea(
	'TEXTAREA1',
	$this->GetModuleVarName('textarea1'),
	'Text Area (Displays an error if it contains the string \'error\')',
	$this->GetModuleVar('textarea1'),
	40,
	5);
	
if (FindString($this->GetModuleVar('textarea1'),'error') != -1)
{ 
	$arrayErrors['TEXTAREA1'] = 'This field must not contain the string \'error\'!';	
}	

$formDef->AddControlPassword(
	'PASSWORD1',
	$this->GetModuleVarName('password1'),
	'Password',
	'',
	20,
	20);
	
$formDef->AddControlSeparator();

$formDef->AddControlDate(
	'DATE1',
	$this->GetModuleVarName('date1'),
	'Date',
	$this->GetModuleVar('date1'));

$arrayOptions = array();
$arrayOptions['option1'] = 'Option 1';
$arrayOptions['option2'] = 'Option 2';
$arrayOptions['option3'] = 'Option 3';

$formDef->AddControlSelect(
	'SELECT1',
	$this->GetModuleVarName('select1'),
	'Selection',
	$arrayOptions,
	$this->GetModuleVar('select1'));
	
if ($this->GetModuleVar('select1') == 'option3')
{
	$arrayErrors['SELECT1'] = 'Option 3 produces an error!';	
}	

$formDef->AddControlRadio(
	'RADIO1',
	$this->GetModuleVarName('radio1'),
	'Radio',
	$arrayOptions,
	$this->GetModuleVar('radio1'));

$formDef->AddControlCheckbox(
	'CHECKBOX1',
	$this->GetModuleVarName('checkbox1'),
	'Checkbox',
	$this->GetModuleVar('checkbox1'));

if ($this->GetModuleVar('checkbox1') == false)
{ 
	$arrayErrors['CHECKBOX1'] = 'The checkbox must be ticked!';	
}	

$formDef->AddControlFile(
	'FILE1',
	$this->GetModuleVarName('file1'),
	'Upload a file'); 
	
$formDef->AddControlButtonReset('Reset');
$formDef->AddControlButtonSubmit('Submit');		



$form = new CFormElement(
	$this->GetWebsite(),
	'default');
	
$strOutput = $text->RenderText(
	'CFormElement::RenderForm',
	$form->RenderForm(
		$formDef,
		$this->GetAllModuleVars(),
		$arrayErrors)
	);



$form = new CFormElement(
	$this->GetWebsite(),
	'onecolumn');
	
$strOutput .= $text->RenderText(
	'CFormElement::RenderForm style=onecolumn',
	$form->RenderForm(
		$formDef,
		$this->GetAllModuleVars(),
		$arrayErrors)
	);


$form = new CFormTemplateElement(
	$this->GetWebsite(),
	'default');
	
$strOutput .= $text->RenderText(
	'CFormTemplateElement::RenderFormTemplate',
	$form->RenderFormTemplate(
		$formDef,
		$this->GetAllModuleVars(),
		$arrayErrors,
		'demo1')
	);
	
	

$arrayErrors = array();
$formDef = new CFormDefinition();

$formDef->SetURL(
	$this->GetWebsite()->GetRootURL(),
	$this->GetWebsite()->GetSessionAwareArrayVars());
	
$formDef->SetHiddenValue(
	$this->GetModuleVarName('somehiddencounter'),
	intval($this->GetModuleVar('somehiddencounter')) + 1);

$formDef->AddControlReadOnly(
	'STATIC1',
	'Hidden counter',
	intval($this->GetModuleVar('somehiddencounter')));

$arrayOptions = array();
$arrayOptions['option1'] = 'Option 1';
$arrayOptions['option2'] = 'Option 2';
$arrayOptions['option3'] = 'Option 3';

$formDef->AddControlSelect(
	'SELECT1',
	$this->GetModuleVarName('select1'),
	'Selection',
	$arrayOptions,
	$this->GetModuleVar('select1'));

if ($this->GetModuleVar('select1') == 'option3')
{
	$arrayErrors['SELECT1'] = 'Option 3 produces an error!';	
}	


$formDef->AddControlText(
	'TEXT2',
	$this->GetModuleVarName('text2'),
	'Text (Displays an error if empty)',
	$this->GetModuleVar('text2'),
	40,
	40);
	
if ($this->GetModuleVar('text2') == '')
{
	$arrayErrors['TEXT2'] = 'This field must not be empty!';	
}	

$formDef->AddControlSeparator();
$formDef->AddControlButtonReset('Reset');
$formDef->AddControlButtonSubmit('Submit');		



$form = new CFormElement(
	$this->GetWebsite(),
	'horizontal');
	
$strOutput .= $text->RenderText(
	'CFormElement::RenderForm style=horizontal',
	$form->RenderForm(
		$formDef,
		$this->GetAllModuleVars(),
		$arrayErrors)
	);
	
	


if ($this->GetWebsite()->GetVar('downloadtoken') != '')
{
	$this->GetWebsite()->DownloadFile(
		'text/plain',
		'test.txt',
		'This is a test');
}
else
{
	
	$arrayErrors = array();
	$formDef = new CFormDefinition();
		
	$formDef->SetURL(
		$this->GetWebsite()->GetRootURL(),
		$this->GetWebsite()->GetSessionAwareArrayVars());
		
	$formDef->AddControlStatic(
		'STATIC1',
		'This form uses the special download button');
	
	$formDef->AddControlButtonSubmitDownload('Download');		
	
	$form = new CFormElement(
		$this->GetWebsite(),
		'default');
		
	$strOutput .= $text->RenderText(
		'CFormElement::RenderForm (with a download button)',
		$form->RenderForm(
			$formDef,
			$this->GetAllModuleVars(),
			$arrayErrors)
		);	
}



$formDef = new CFormDefinition();

$formDef->SetURL(
	$this->GetWebsite()->GetRootURL(),
	$this->GetWebsite()->GetSessionAwareArrayVars());

$formDef->AddControlText(
	'TEXT3',
	$this->GetModuleVarName('text3'),
	'Text (Displays an error if empty)',
	$this->GetModuleVar('text3'),
	40,
	40);

$formDef->AddControlButtonSubmit('Submit');		
	
if ($this->GetModuleVar('text3') == '')
{
	$arrayErrors['TEXT3'] = 'This field must not be empty!';	
}	

$form = new CFormElement(
	$this->GetWebsite(),
	'compact');
	
$strOutput .= $text->RenderText(
	'CFormElement::RenderForm style=compact',
	$form->RenderForm(
		$formDef,
		$this->GetAllModuleVars(),
		$arrayErrors)
	);