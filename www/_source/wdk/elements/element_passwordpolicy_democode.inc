$this->IncludeElement("passwordpolicy");

$passwordpolicy = new CPasswordPolicy();
$passwordpolicy->SetMinimumLength(4);
$passwordpolicy->SetRequiredCharsAlpha();
$passwordpolicy->SetRequiredCharsNumeric();

$passwordpolicyelement = new CPasswordPolicyElement(
	$this->GetWebsite(),"default");
$strOutput = $passwordpolicyelement->RenderPasswordPolicyInfo(
	$passwordpolicy);

$arrayErrors = array();
$passwordpolicy->CheckPassword("%",$arrayErrors);
$strOutput .= $passwordpolicyelement->RenderPasswordPolicyErrors(
	$arrayErrors);