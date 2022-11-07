#include "Exceptions.h"


void Exceptions::InitHandlers(_se_translator_function CustomTranslator) {
	_set_se_translator(CustomTranslator);
}


void __cdecl SE_TranslatorFunction(unsigned int ExceptionCode, EXCEPTION_POINTERS* ExceptionInfo) {
	throw Exceptions::Exception(ExceptionCode, *ExceptionInfo->ContextRecord, *ExceptionInfo->ExceptionRecord);
}

LONG WINAPI UEF_HandlerFunction(EXCEPTION_POINTERS* ExceptionInfo) {
	return EXCEPTION_CONTINUE_SEARCH;
}

void Exceptions::InitHandlers() {
	_set_se_translator(SE_TranslatorFunction);
	SetUnhandledExceptionFilter(UEF_HandlerFunction);
}
