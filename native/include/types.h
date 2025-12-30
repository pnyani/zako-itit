#ifndef TYPES_H
#define TYPES_H

typedef enum {
    TAG_BUG = 0,
    TAG_FEATURE = 1,
    TAG_ENHANCEMENT = 2
} IssueTag;

typedef enum {
    STATUS_PROPOSED = 0,
    STATUS_APPROVED = 1,
    STATUS_REJECTED = 2,
    STATUS_DELETED = 3
} IssueStatus;

typedef struct {
    int id;
    char name[256];
    char detail[2048];
    IssueTag tag;
    IssueStatus status;
    char user_id[64];
} Issue;

#endif // TYPES_H
