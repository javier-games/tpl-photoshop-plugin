#include <pidefines.h>
#include <piabout.h>
#include <pifilter.h>
#include <spbasic.h>


SPBasicSuite *sSPBasic = NULL;


void executeFilter(const FilterRecordPtr &filterRecord)
{
    // TODO: implement
    return;
}


DLLExport MACPASCAL void PluginMain(const int16_t selector,
                                    FilterRecordPtr filterRecord,
                                    long long *data,
                                    int16_t *result)
{
    switch (selector) {
    case filterSelectorAbout:
        sSPBasic = ((AboutRecord *)filterRecord)->sSPBasic;

    case filterSelectorStart:
        {
            sSPBasic = filterRecord->sSPBasic;
            if (filterRecord->bigDocumentData != NULL) {
                filterRecord->bigDocumentData->PluginUsing32BitCoordinates = true;
            }

            executeFilter(filterRecord);

            break;
        }
    default:
        break;
    }

    return;
}