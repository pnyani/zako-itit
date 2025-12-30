# zako-itit

A hybrid TypeScript + C Discord bot for issue tracking.

## Tech Stack
- Discord API: TypeScript with discord.js
- Core Logic: C native addon (N-API)
- Database: SQLite

## Requirements
- Node.js >= 18.0.0
- SQLite3 development libraries
- C compiler (gcc/clang)

## Docker

```bash
docker pull nyanni/zako-itit
```

### Docker Compose
```yaml
services:
  bot:
    image: nyanni/zako-itit
    container_name: zako-itit-bot
    restart: unless-stopped
    environment:
      - DISCORD_TOKEN=${DISCORD_TOKEN}
      - SQLITE_FILE=/app/data/issues.db
      - ADMIN_IDS=${ADMIN_IDS:-}
      - EMBED_PAGE_SIZE=${EMBED_PAGE_SIZE:-5}
    volumes:
      - ./data:/app/data
```

### Run with Docker
```bash
docker run -d \
  --name zako-bot \
  -e DISCORD_TOKEN=your_token \
  -v ./data:/app/data \
  nyanni/zako-itit
```

## Installation (Local)
```bash
npm install
npm run build
```

## Environment Variables
| Variable | Description | Default |
|----------|-------------|---------|
| `DISCORD_TOKEN` | Discord bot token | Required |
| `SQLITE_FILE` | Path to SQLite database | `./data/issues.db` |
| `ADMIN_IDS` | Comma-separated admin user IDs | - |
| `EMBED_PAGE_SIZE` | Number of issues per page | `5` |

## Commands

### `/ping`
Check if the bot is alive.

### `/issue new`
Create a new issue via modal dialog.
- Fields: Name, Tag (Bug/Feature/Enhancement), Detail

### `/issue get <id>`
Get details of a specific issue by ID.

### `/issue list [tag] [status]`
List all issues with optional filters.
- Tags: Bug (0), Feature (1), Enhancement (2)
- Status: Proposed (0), Approved (1), Rejected (2), Deleted (3)

### `/issue set-status <id> <status>` (Admin only)
Update the status of an issue.

### `/issue delete <id>` (Admin only)
Delete an issue.

### `/issue export [tag]`
Export issues as JSON. Outputs as code block if under 2000 chars, otherwise as file attachment.

## Database Schema
```sql
CREATE TABLE IF NOT EXISTS zako (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  tag INTEGER NOT NULL,
  status INTEGER NOT NULL,
  name TEXT NOT NULL,
  detail TEXT NOT NULL,
  discord TEXT NOT NULL
);
```

## License
MIT - pnyani
