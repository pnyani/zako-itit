#include "utils.h"

const char* tag_to_string(IssueTag tag) {
    switch (tag) {
        case TAG_BUG: return "Bug";
        case TAG_FEATURE: return "Feature";
        case TAG_ENHANCEMENT: return "Enhancement";
        default: return "Unknown";
    }
}

const char* status_to_string(IssueStatus status) {
    switch (status) {
        case STATUS_PROPOSED: return "Proposed";
        case STATUS_APPROVED: return "Approved";
        case STATUS_REJECTED: return "Rejected";
        case STATUS_DELETED: return "Deleted";
        default: return "Unknown";
    }
}
