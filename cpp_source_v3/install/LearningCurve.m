function LearningCurve(meanReturns, errorbarValues, curveNames, episodeNumbers)
    [numAlgs, maxEps] = size(meanReturns);
    x = episodeNumbers;

    hold on;
    
    % Array of line colors
    colors = [ ...
        0    0.4470    0.7410; ...
        0.9, 0.1, 0.1; ...
        0.4660, 0.6740, 0.1880; ...
        0.6940, 0.3840, 0.7560;...
        0.2500, 0.2500, 0.2500; ...
        0.9290, 0.6940, 0.1250; ...
        ];
    nColors = size(colors,1);
    
    % Draw the error bar ranges first
    for i = 1:numAlgs
        y = meanReturns(i,:)';
        dy = errorbarValues(i,:)';
        fill([x;flipud(x)],[y-dy;flipud(y+dy)],colors(mod(i-1,nColors)+1,:), ...
            'linestyle','none');
        alpha(0.1);
    end
    
    % Draw the curves
    for i = 1:numAlgs
        y = meanReturns(i,:)';
        colorIndex = i-1; % 0-based indexing so we can use mods
        while (floor(colorIndex / nColors) > 3)
            colorIndex = colorIndex - 4*nColors; % After we run out of line types, loop back around and re-use some
        end
        if (floor(colorIndex/nColors) == 0)
            p(i) = plot(x,y, 'Color', colors(mod(colorIndex,nColors)+1,:), 'LineWidth', 1);
        elseif (floor(colorIndex/nColors) == 1)
            p(i) = plot(x,y, ':', 'Color', colors(mod(colorIndex,nColors)+1,:), 'LineWidth', 1);
        elseif (floor(colorIndex/ncolors) == 2)
            p(i) = plot(x,y, '--', 'Color', colors(mod(colorIndex,nColors)+1,:), 'LineWidth', 1);
        elseif (floor(colorIndex/ncolors) == 3)
            p(i) = plot(x,y, '-.', 'Color', colors(mod(colorIndex,nColors)+1,:), 'LineWidth', 1);
        end
  
    end
    
    % Labels
    xlabel('Episodes');
    ylabel('Expected Return');
    
    legend(p(1:numAlgs),curveNames, 'Location', 'SouthEast');
    
    % Make pretty
    box off;
    set(gcf,'color','w');
    xlim([1,max(episodeNumbers)]);
end