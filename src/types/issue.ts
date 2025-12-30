export enum IssueTag {
  Bug = 0,
  Feature = 1,
  Enhancement = 2
}

export enum IssueStatus {
  Proposed = 0,
  Approved = 1,
  Rejected = 2,
  Deleted = 3
}

export interface Issue {
  id: number;
  name: string;
  detail: string;
  tag: IssueTag;
  status: IssueStatus;
  userId: string;
}

export const TAG_NAMES: Record<number, string> = {
  0: 'Bug',
  1: 'Feature',
  2: 'Enhancement'
};

export const STATUS_NAMES: Record<number, string> = {
  0: 'Proposed',
  1: 'Approved',
  2: 'Rejected',
  3: 'Deleted'
};
