import { ActionRowBuilder, ButtonBuilder, ButtonStyle } from 'discord.js';

export function createPaginationButtons(
  currentPage: number,
  hasPrevious: boolean,
  hasNext: boolean,
  tag?: number | null,
  status?: number | null
): ActionRowBuilder<ButtonBuilder> {
  const customIdPrefix = 'pagination';
  const tagParam = tag !== null && tag !== undefined ? tag.toString() : 'null';
  const statusParam = status !== null && status !== undefined ? status.toString() : 'null';

  const previousButton = new ButtonBuilder()
    .setCustomId(`${customIdPrefix}:prev:${currentPage}:${tagParam}:${statusParam}`)
    .setLabel('Previous')
    .setStyle(ButtonStyle.Primary)
    .setDisabled(!hasPrevious);

  const nextButton = new ButtonBuilder()
    .setCustomId(`${customIdPrefix}:next:${currentPage}:${tagParam}:${statusParam}`)
    .setLabel('Next')
    .setStyle(ButtonStyle.Primary)
    .setDisabled(!hasNext);

  return new ActionRowBuilder<ButtonBuilder>().addComponents(previousButton, nextButton);
}
