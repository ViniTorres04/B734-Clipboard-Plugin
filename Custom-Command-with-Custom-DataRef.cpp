#define XPLM200
#define XPLM210
#include "XPLMDefs.h"
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"
#include "XPLMDataAccess.h"
// ... outros includes

// Variáveis globais
XPLMDataRef gClipboardDataRef = NULL;
XPLMCommandRef gToggleClipboardCmd = NULL;
int gClipboardState = 0; // 0 = Escondido, 1 = Visível

// Protótipos das funções
int GetClipboardCB(void* inRefcon);
void SetClipboardCB(void* inRefcon, int inValue);
int ToggleClipboardHandler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon);

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc) {
    strcpy(outName, "FC B734 Clipboard Control");
    strcpy(outSig, "FC.B734.clipboard_control");
    strcpy(outDesc, "Controla a visibilidade da prancheta do B734.");

    // 1. Registrar o DataRef (FC/B734/clipboard)
    gClipboardDataRef = XPLMRegisterDataAccessor("FC/B734/clipboard",
        xplmType_Int,                                  // Tipo Inteiro
        1,                                             // Escrita permitida
        GetClipboardCB, SetClipboardCB,                // Accessors para Int
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    // 2. Criar o Comando (FC/B734/toggle_clipboard)
    gToggleClipboardCmd = XPLMCreateCommand("FC/B734/toggle_clipboard", "Alternar Visibilidade da Prancheta");

    // 3. Registrar o Handler do Comando
    XPLMRegisterCommandHandler(gToggleClipboardCmd, ToggleClipboardHandler, 1, (void*)0);

    return 1;
}

// Accessors para o DataRef
int GetClipboardCB(void* inRefcon) { return gClipboardState; }
void SetClipboardCB(void* inRefcon, int inValue) { gClipboardState = inValue; }

// Handler do Comando (Lógica de Toggle)
int ToggleClipboardHandler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon) {
    // Usar 0 aqui resolve qualquer erro de "identificador não encontrado"
    if (inPhase == 0) {
        if (gClipboardState == 0) {
            gClipboardState = 1;
        }
        else {
            gClipboardState = 0;
        }
    }
    return 0;
}

PLUGIN_API void XPluginStop(void) {
    XPLMUnregisterDataAccessor(gClipboardDataRef);
    XPLMUnregisterCommandHandler(gToggleClipboardCmd, ToggleClipboardHandler, 0, 0);
}

PLUGIN_API void XPluginDisable(void) {}
PLUGIN_API int XPluginEnable(void) { return 1; }
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFromWho, long inMessage, void* inParam) {}