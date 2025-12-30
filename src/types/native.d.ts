import { Issue, IssueTag, IssueStatus } from './issue';

export interface NativeAddon {
  initDatabase(dbPath: string): boolean;
  closeDatabase(): void;
  createIssue(name: string, detail: string, tag: IssueTag, userId: string): number;
  getIssue(id: number): Issue | null;
  listIssues(tag?: IssueTag | null, status?: IssueStatus | null): Issue[];
  updateIssueStatus(id: number, newStatus: IssueStatus): boolean;
  deleteIssue(id: number): boolean;
}

declare const addon: NativeAddon;
export default addon;
