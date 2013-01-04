#include <znc/main.h>
#include <znc/Modules.h>
#include <algorithm>

class CRom : public CModule
{
public:
    MODCONSTRUCTOR(CRom) {
        AddHelpCommand();
        AddCommand("Add",
                   static_cast<CModCommand::ModCmdFunc>(&CRom::addChannelCommand),
                   "<channel>");
        AddCommand("Del",
                   static_cast<CModCommand::ModCmdFunc>(&CRom::delChannelCommand),
                   "<channel>");
        AddCommand("List",
                   static_cast<CModCommand::ModCmdFunc>(&CRom::listChannelCommand));
    }

    virtual ~CRom() {}

    /**
     * @verbatim
     *
     *   /msg *status loadmod rom #hogechan
     *   /msg *status loadmod rom #hogechan #fugachan
     *
     * @endverbatim
     */
    virtual bool OnLoad(const CString& sArgs, CString& sMessage) {
        sArgs.Split(" ", channels);
        return true;
    }

    /**
     * @verbatim
     *   /msg *rom add #fuga
     *   /msg *rom add #hago
     *   /msg *rom add #fuga // => Notice "#fuga is already been added."
     * @endverbatim
     *
     * @param  sLine  Command String
     */
    void addChannelCommand(const CString &sLine) {
        CString channel(sLine.Token(1));

        if (isReadOnlyChannel(channel)) {
            PutModNotice(channel + " is already been added.");
            return;
        }

        addChannel(channel);
        printChannel();
    }

    /**
     * @verbatim
     *   /msg *rom del #fuga
     *   /msg *rom del #hoge
     * @endverbatim
     *
     * @param  sLine  Command String
     */
    void delChannelCommand(const CString &sLine) {
        CString channel(sLine.Token(1));
        delChannel(channel);
        printChannel();
    }

    /**
     * @verbatim
     *   /msg *rom list
     *     => #hoge #fuga #hago
     * @endverbatim
     *
     * @param  sLine  Command String
     */
    void listChannelCommand(const CString &sLine) {
        printChannel();
    }

    virtual EModRet OnUserMsg(CString& sTarget, CString& sMessage) {
        return sendUserEventHook(sTarget, sMessage);
    }

    virtual EModRet OnUserNotice(CString& sTarget, CString& sMessage) {
        return sendUserEventHook(sTarget, sMessage);
    }

private:

    /**
     * List of read-only channel
     */
    VCString channels;

    /**
     * Run after input message or notice (before send to IRC server).
     *
     * @return  CONTINUE if this channel is not read-only. Otherwise HALT.
     */
    EModRet sendUserEventHook(CString& sTarget, CString& sMessage) {
        if (isReadOnlyChannel(sTarget)) {
            PutModNotice(sTarget + " is read-only channel.");
            return HALT;
        }

        return CONTINUE;
    }

    /**
     * Check if this channel has already been added.
     *
     * @code
     *   printChannel(); // => "#hoge #fuga #hago"
     *   isReadOnlyChannel("#fuga");  // => true
     *   isReadOnlyChannel("#hago");  // => true
     *   isReadOnlyChannel("#abcde"); // => false
     * @endcode
     *
     * @return True if this channel has already been added.
     */
    bool isReadOnlyChannel(const CString chan) {
        VCString::iterator pos;
        pos = std::find(channels.begin(), channels.end(), chan);
        return !(pos == channels.end());
    }

    /**
     * Add read-only channel
     *
     * @param  newChannel  Name of read-only channel.
     */
    void addChannel(const CString chan) {
        channels.push_back(chan);
    }

    /**
     * Delete read-only channel
     *
     * @param  newChannel  Name of read-only channel.
     */
    void delChannel(const CString chan) {
        VCString::iterator it = std::remove(channels.begin(), channels.end(), chan);
        channels.erase(it, channels.end());
    }

    /**
     * Print read-only channel
     */
    void printChannel() {
        CString output("current read-only channels: ");

        for (VCString::iterator it = channels.begin(); it != channels.end(); ++it) {
            output += (*it + " ");
        }

        PutModNotice(output);
    }
};

USERMODULEDEFS(CRom, "Realized the ROM (Read Only Member)");
