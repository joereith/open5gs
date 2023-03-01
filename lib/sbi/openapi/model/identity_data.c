
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "identity_data.h"

OpenAPI_identity_data_t *OpenAPI_identity_data_create(
    OpenAPI_list_t *supi_list,
    OpenAPI_list_t *gpsi_list
)
{
    OpenAPI_identity_data_t *identity_data_local_var = ogs_malloc(sizeof(OpenAPI_identity_data_t));
    ogs_assert(identity_data_local_var);

    identity_data_local_var->supi_list = supi_list;
    identity_data_local_var->gpsi_list = gpsi_list;

    return identity_data_local_var;
}

void OpenAPI_identity_data_free(OpenAPI_identity_data_t *identity_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == identity_data) {
        return;
    }
    if (identity_data->supi_list) {
        OpenAPI_list_for_each(identity_data->supi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(identity_data->supi_list);
        identity_data->supi_list = NULL;
    }
    if (identity_data->gpsi_list) {
        OpenAPI_list_for_each(identity_data->gpsi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(identity_data->gpsi_list);
        identity_data->gpsi_list = NULL;
    }
    ogs_free(identity_data);
}

cJSON *OpenAPI_identity_data_convertToJSON(OpenAPI_identity_data_t *identity_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (identity_data == NULL) {
        ogs_error("OpenAPI_identity_data_convertToJSON() failed [IdentityData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (identity_data->supi_list) {
    cJSON *supi_listList = cJSON_AddArrayToObject(item, "supiList");
    if (supi_listList == NULL) {
        ogs_error("OpenAPI_identity_data_convertToJSON() failed [supi_list]");
        goto end;
    }
    OpenAPI_list_for_each(identity_data->supi_list, node) {
        if (cJSON_AddStringToObject(supi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_identity_data_convertToJSON() failed [supi_list]");
            goto end;
        }
    }
    }

    if (identity_data->gpsi_list) {
    cJSON *gpsi_listList = cJSON_AddArrayToObject(item, "gpsiList");
    if (gpsi_listList == NULL) {
        ogs_error("OpenAPI_identity_data_convertToJSON() failed [gpsi_list]");
        goto end;
    }
    OpenAPI_list_for_each(identity_data->gpsi_list, node) {
        if (cJSON_AddStringToObject(gpsi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_identity_data_convertToJSON() failed [gpsi_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_identity_data_t *OpenAPI_identity_data_parseFromJSON(cJSON *identity_dataJSON)
{
    OpenAPI_identity_data_t *identity_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi_list = NULL;
    OpenAPI_list_t *supi_listList = NULL;
    cJSON *gpsi_list = NULL;
    OpenAPI_list_t *gpsi_listList = NULL;
    supi_list = cJSON_GetObjectItemCaseSensitive(identity_dataJSON, "supiList");
    if (supi_list) {
        cJSON *supi_list_local = NULL;
        if (!cJSON_IsArray(supi_list)) {
            ogs_error("OpenAPI_identity_data_parseFromJSON() failed [supi_list]");
            goto end;
        }

        supi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_list_local, supi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supi_list_local)) {
                ogs_error("OpenAPI_identity_data_parseFromJSON() failed [supi_list]");
                goto end;
            }
            OpenAPI_list_add(supi_listList, ogs_strdup(supi_list_local->valuestring));
        }
    }

    gpsi_list = cJSON_GetObjectItemCaseSensitive(identity_dataJSON, "gpsiList");
    if (gpsi_list) {
        cJSON *gpsi_list_local = NULL;
        if (!cJSON_IsArray(gpsi_list)) {
            ogs_error("OpenAPI_identity_data_parseFromJSON() failed [gpsi_list]");
            goto end;
        }

        gpsi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_list_local, gpsi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(gpsi_list_local)) {
                ogs_error("OpenAPI_identity_data_parseFromJSON() failed [gpsi_list]");
                goto end;
            }
            OpenAPI_list_add(gpsi_listList, ogs_strdup(gpsi_list_local->valuestring));
        }
    }

    identity_data_local_var = OpenAPI_identity_data_create (
        supi_list ? supi_listList : NULL,
        gpsi_list ? gpsi_listList : NULL
    );

    return identity_data_local_var;
end:
    if (supi_listList) {
        OpenAPI_list_for_each(supi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(supi_listList);
        supi_listList = NULL;
    }
    if (gpsi_listList) {
        OpenAPI_list_for_each(gpsi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(gpsi_listList);
        gpsi_listList = NULL;
    }
    return NULL;
}

OpenAPI_identity_data_t *OpenAPI_identity_data_copy(OpenAPI_identity_data_t *dst, OpenAPI_identity_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_identity_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_identity_data_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_identity_data_free(dst);
    dst = OpenAPI_identity_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

