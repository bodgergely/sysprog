EXTERN(preinits)
SECTIONS
{
	. = ALIGN(0x200000);
	PROVIDE_HIDDEN(__start_text = .);
}
INSERT BEFORE .text


SECTIONS
{
	. = ALIGN(0x200000);
	PROVIDE_HIDDEN(__end_text = .);
}
INSERT AFTER .text
